using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Lab5_TaskWrapping
{
	public static class FilePaths
	{
		//public static String source = "notepad-plus-plus.org";
		//public static List<String> paths = new List<string>
		//{
		//    "/repository/7.x/7.5.1/npp.7.5.1.Installer.x64.exe"
		//};
		public static String source = "www.cs.ubbcluj.ro";
		public static List<String> paths = new List<string>
		{
			"/~rares/course/vr/lighting-algorithm.png",
			"/~rares/course/vr/with-lighting.png"
		};
	}

	// State object for receiving data from remote device.  
	public class StateObject
	{
		// Client socket.  
		public Socket WorkSocket;
		// Size of receive buffer.  
		public const int BufferSize = 512;
		// Receive buffer.  
		public readonly byte[] Buffer = new byte[BufferSize];
		// Received data string.  
		public readonly StringBuilder Sb = new StringBuilder();
	}

    public class AsynchronousClient
    {
        private string sourcePage;
        private string targetFile;
        private const int Port = 80;
        private string getHeader;

        private ManualResetEvent receiveDone =
            new ManualResetEvent(false);

		public AsynchronousClient(string sourcePage, string targetFile)
		{
			this.sourcePage = sourcePage;
			this.targetFile = targetFile;
			this.getHeader = "GET " + this.targetFile + " HTTP/1.1\r\n" +
				"Host: www.cs.ubbcluj.ro\r\n\r\n";
			//Console.WriteLine("Request: " + this.getHeader);
			
		}

        public void waitForReceive(){
            this.receiveDone.WaitOne();
        }

		public void StartClient()
		{
			var ipHostInfo = Dns.GetHostEntry(this.sourcePage);
			var ipAddress = ipHostInfo.AddressList[0];
			var remoteEp = new IPEndPoint(ipAddress, Port);
			var client = new Socket(ipAddress.AddressFamily,
				SocketType.Stream, ProtocolType.Tcp);
			Task.Factory.StartNew(() => Connect(client, remoteEp))
                .ContinueWith(t => Send(t.Result.Result, this.getHeader))
				.ContinueWith(t => Receive(t.Result.Result))
				.ContinueWith(t => {
                    string[] tokens = t.Result.Result.Split(new string[] { "\r\n" }, StringSplitOptions.None);
					foreach (string s in tokens)
					{
						if (s.Equals(""))
							break;
						if (s.Contains("Content-Length"))
							//Console.WriteLine(s);
							Console.WriteLine("The downloaded file will have " + s.Split(':')[1] + " bytes!");
						else
							Console.WriteLine(s);
					}
					Console.WriteLine("\n\n");
                });
		}

		private Task<Socket> Connect(Socket client, EndPoint remoteEp)
		{
			var tcs = new TaskCompletionSource<Socket>();
			// Connect to the remote endpoint.  
			client.BeginConnect(remoteEp,
				iar =>
				{
					var socket = (Socket)iar.AsyncState;

					// Complete the connection.  
					socket.EndConnect(iar);

					Console.WriteLine("Socket connected to {0}",
						client.RemoteEndPoint);
					try
					{
						tcs.TrySetResult(socket);
					}
					catch (Exception ex)
					{
						tcs.TrySetException(ex);
					}
				}, client);
			return tcs.Task;
		}

		private Task<Socket> Send(Socket client, string data)
		{
			var tcs = new TaskCompletionSource<Socket>();
			var byteData = Encoding.ASCII.GetBytes(data);
	
			client.BeginSend(byteData, 0, byteData.Length, 0,
				iar =>
				{
					// Retrieve the socket from the state object.  
					var cl = (Socket)iar.AsyncState;

					// Complete sending the data to the remote device.  
					var bytesSent = cl.EndSend(iar);
					Console.WriteLine("Sent {0} bytes to server.", bytesSent);

					tcs.SetResult(cl);
				}, client);

			return tcs.Task;
		}

		private Task<string> Receive(Socket client)
		{
			var tcs = new TaskCompletionSource<string>();
			try
			{
                var state = new StateObject { WorkSocket = client };
				// Begi receiving the data from the remote device.  
                client.BeginReceive(state.Buffer, 0, StateObject.BufferSize, 0,
					iar =>
					{
                        var st = (StateObject)iar.AsyncState;
						var cl = state.WorkSocket;

						// Read data from the remote device.  
						var bytesRead = client.EndReceive(iar);
						// There might be more data, so store the data received so far.  
						st.Sb.Append(Encoding.ASCII.GetString(state.Buffer, 0, bytesRead));
						// Signal that all bytes have been received.  
						client.Shutdown(SocketShutdown.Both);
						client.Close();

						tcs.SetResult(st.Sb.ToString());
                        this.receiveDone.Set();
					}, state);
			}
			catch (Exception e)
			{
				Console.WriteLine(e.ToString());
			}

			return tcs.Task;
		}

	}

	public class Program
	{
		public static void Main(string[] args)
		{
            List<AsynchronousClient> clients = new List<AsynchronousClient>();
			foreach (String s in FilePaths.paths)
			{
                AsynchronousClient client = new AsynchronousClient(FilePaths.source, s);
				client.StartClient();
                client.waitForReceive();
			}
            Console.ReadKey();
            //foreach (AsynchronousClient client in clients){
            //    client.waitForReceive();
            //}
		}
	}
}