using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace server
{
    class Program
    {
        static void Main(string[] args)
        {
            int port = 8080;
            string ip = "127.0.0.1";
            IPEndPoint ipPoint = new IPEndPoint(IPAddress.Parse(ip), port);
            Socket soket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            soket.Bind(ipPoint);
            soket.Listen(3);
            Console.WriteLine("Сервер запущен. Ожидание подключений...");
            while (true)
            {
                Socket listener = soket.Accept();
                StringBuilder message = new StringBuilder();
                int bytes = 0;
                byte[] buffer = new byte[256];
                do
                {
                    bytes = listener.Receive(buffer);
                    message.Append(Encoding.UTF8.GetString(buffer, 0, bytes));
                }
                while (listener.Available > 0);
                Console.WriteLine(DateTime.Now.ToShortTimeString() + ":" + message.ToString());

                string answer = "ок понял";
                buffer = Encoding.UTF8.GetBytes(answer);
                listener.Send(buffer);

                listener.Shutdown(SocketShutdown.Both);
                listener.Close();
            }
        }
    }
}

