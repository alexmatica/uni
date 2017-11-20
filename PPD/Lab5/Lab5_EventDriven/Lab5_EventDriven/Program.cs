using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Collections.Generic;

namespace Lab5_EventDriven
{
    public static class FilePaths{
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

		private string _response = string.Empty;
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

		public void StartClient()
		{
            var ipHostInfo = Dns.GetHostEntry(this.sourcePage);
			var ipAddress = ipHostInfo.AddressList[0];
			var remoteEp = new IPEndPoint(ipAddress, Port);
			var client = new Socket(ipAddress.AddressFamily,
				SocketType.Stream, ProtocolType.Tcp);
			Connect(client, remoteEp);
            this.receiveDone.WaitOne();
		}

		private void Connect(Socket client, EndPoint remoteEp)
		{
			client.BeginConnect(remoteEp,
				ConnectCallback, client);
		}

		private void ConnectCallback(IAsyncResult ar)
		{
			try
			{ 
				var client = (Socket)ar.AsyncState;
				client.EndConnect(ar);

				Console.WriteLine("Socket connected to {0}",
					client.RemoteEndPoint);
                                    
                Send(client, this.getHeader);
			}
			catch (Exception e)
			{
				Console.WriteLine(e.ToString());
			}
		}

		private void Send(Socket client, string data)
		{
			var byteData = Encoding.ASCII.GetBytes(data);
			client.BeginSend(byteData, 0, byteData.Length, 0,
				SendCallback, client);
		}

		private void SendCallback(IAsyncResult ar)
		{
			try
			{
				var client = (Socket)ar.AsyncState;
				var bytesSent = client.EndSend(ar);
				Console.WriteLine("Sent {0} bytes to server.", bytesSent);
				Receive(client);

			}
			catch (Exception e)
			{
				Console.WriteLine(e.ToString());
			}
		}

		private void Receive(Socket client)
		{
			try
			{  
                var state = new StateObject { WorkSocket = client };  
                client.BeginReceive(state.Buffer, 0, StateObject.BufferSize, 0,
					ReceiveCallback, state);
			}
			catch (Exception e)
			{
				Console.WriteLine(e.ToString());
			}
		}

        private void ReceiveCallback(IAsyncResult ar)
        {
            try
            {
                // Retrieve the state object and the client socket   
                // from the asynchronous state object.  
                var state = (StateObject)ar.AsyncState;
                var client = state.WorkSocket;

                // Read data from the remote device.  
                var bytesRead = client.EndReceive(ar);
                if (bytesRead > 0 && state.Sb.Length < 512)
                {
                    // There might be more data, so store the data received so far.  
                    state.Sb.Append(Encoding.ASCII.GetString(state.Buffer, 0, bytesRead));

                    // Get the rest of the data.  
                    client.BeginReceive(state.Buffer, 0, StateObject.BufferSize, 0,
                        ReceiveCallback, state);
                }
                else
                {
                    // All the data has arrived; put it in response.  
                    if (state.Sb.Length > 1)
                    {
                        _response = state.Sb.ToString();
                    }
                    // Signal that all bytes have been received.  
                    client.Shutdown(SocketShutdown.Both);
                    client.Close();
                    string[] tokens = this._response.Split(new string[] { "\r\n" }, StringSplitOptions.None);
                    foreach(string t in tokens){
                        if (t.Equals(""))
                            break;
                        if (t.Contains("Content-Length"))
                            Console.WriteLine("The downloaded file will have " + t.Split(':')[1] + " bytes!");
                        else
                            Console.WriteLine(t);
                    }
                    Console.WriteLine("\n\n");
                    this.receiveDone.Set();

                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }
	}

    public class Program{
        public static void Main(string[] args){
            foreach (String s in FilePaths.paths)
            {
                var client = new AsynchronousClient(FilePaths.source, s);
                client.StartClient();
            }
        }
    }
}