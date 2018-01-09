#include <mpi.h>
#include <iostream>
#include <mutex>
#include <vector>
#include <queue>
#include <thread>
#include <unistd.h>
#include <ctime>

using namespace std;

struct ChangeInfo{
	int senderId;
	int variable;
	int value;
	int compare;
};

int procId, nrProcs;
int mem[5][5];
bool done = false;
queue<ChangeInfo> notifyQueue;
mutex mtx;

vector<ChangeInfo> get_changes(int source, int tag){
	int n;
	MPI_Recv(&n, 1, MPI_INT, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	if (n == 0)
		return vector<ChangeInfo>();

	vector<ChangeInfo> changes;
	for (int i=0; i<n; i++){
		ChangeInfo changeInfo;
		MPI_Status status;
		MPI_Recv(&changeInfo, sizeof(ChangeInfo) / sizeof(int), MPI_INT, source, tag, MPI_COMM_WORLD, &status);
		changes.push_back(changeInfo);
	}
	return changes;
}

void send_changes(vector<ChangeInfo> changes, int tag){
	int n = changes.size();
	MPI_Send(&n, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);

	if (n == 0)
		return;

	for (int i=0; i<n; i++)
		MPI_Send(&changes[i], sizeof(ChangeInfo) / sizeof(int), MPI_INT, 0, tag, MPI_COMM_WORLD);	
}

void broadcast_send(vector<ChangeInfo> changes){
	int n = changes.size();
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (n == 0)
		return;

	
	mtx.lock();
	for (int i=0; i<changes.size(); i++){
		// cout << changes[i].senderId << " " << changes[i].variable << " " << changes[i].value << "\n" << flush;
		if (changes[i].compare == -1 || changes[i].compare == mem[changes[i].senderId][changes[i].variable])
			mem[changes[i].senderId][changes[i].variable] = changes[i].value;
		MPI_Bcast(&changes[i], sizeof(ChangeInfo) / sizeof(int), MPI_INT, 0, MPI_COMM_WORLD);
	}
	mtx.unlock();
}

void broadcast_receive(){
	int n;
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (n == 0)
		return;

	mtx.lock();
	ChangeInfo changeInfo;
	for (int i=0; i<n; i++){
		MPI_Bcast(&changeInfo, sizeof(ChangeInfo) / sizeof(int), MPI_INT, 0, MPI_COMM_WORLD);
		if (changeInfo.compare == -1 || changeInfo.compare == mem[changeInfo.senderId][changeInfo.variable])
			mem[changeInfo.senderId][changeInfo.variable] = changeInfo.value;
	}
	mtx.unlock();
}

void poll_master(){
	int tag = 0;
	while(!done){
		vector<ChangeInfo> changes;
		mtx.lock();
		while(notifyQueue.size() > 0){
			changes.push_back(notifyQueue.front());
			notifyQueue.pop();
		}
		mtx.unlock();

		broadcast_send(changes);
		// cout << "Master finished broadcasting changes\n" << flush;

		for (int i = 1; i<nrProcs; i++){
			vector<ChangeInfo> changes = get_changes(i, tag);
			if (changes.size() > 0){
				mtx.lock();
				for (int j=0; j<changes.size(); j++)
					notifyQueue.push(changes[j]);
				mtx.unlock();
			}
		}

		// cout << "Master finished a polling cycle\n" << flush;
		tag++;
	}
}

void poll_worker(){
	int tag = 0;
	while(!done){
		broadcast_receive();
		// cout << "Worker " << procId << " finished receiveng broadcasted changes\n" << flush;
		vector<ChangeInfo> changes;
		mtx.lock();
		while(notifyQueue.size() > 0){
			changes.push_back(notifyQueue.front());
			notifyQueue.pop();
		}
		mtx.unlock();
		send_changes(changes, tag);

		// cout << "Worker " << procId << " finished sending changes\n" << flush;
		tag++;
	}
}

void write_value(int senderId, int variable, int value){
	ChangeInfo changeInfo;
	changeInfo.senderId = senderId;
	changeInfo.variable = variable;
	changeInfo.value = value;
	changeInfo.compare = -1;
	mtx.lock();
	notifyQueue.push(changeInfo);
	mtx.unlock();
}

void query_value(int senderId, int variable){
	cout << "[I AM " << procId << "] Process " << senderId << " on variable " << variable << " -> " << mem[senderId][variable] << "\n" << flush;
}

void process_0(){
	write_value(1, 2, 888);
	usleep(10000);
	query_value(1, 2);
	usleep(10000);
	write_value(1, 2, 999);
	usleep(10000);
	query_value(2, 2);
}

void process_1(){
	usleep(20000);
	query_value(1, 2);
	write_value(2, 2, 666);
	usleep(50000);
	query_value(2, 2);
	usleep(50000);
	query_value(1, 2);
}

void process_2(){
	usleep(20000);
	query_value(1, 2);
	usleep(30000);
	query_value(1, 2);
	write_value(2, 2, 420);
	usleep(20000);
	query_value(2, 2);
}

int main(){
	MPI_Init(NULL, NULL);

	MPI_Comm_rank(MPI_COMM_WORLD, &procId);
	MPI_Comm_size(MPI_COMM_WORLD, &nrProcs);

	if (procId == 0){
		thread master(poll_master);
		process_0();
		usleep(2000000);
		cout << "Master done\n"<<flush;
		done = true;
		master.join();
	} else{
		thread worker(poll_worker);
		switch(procId){
			case 1:
			process_1(); break;
			case 2:
			process_2(); break;
			default:	break;
		}
		usleep(2000000);
		cout << "Worker " << procId << " done \n" << flush;
		done = true;
		worker.join();
	}

	MPI_Finalize();
	return 0;
}

