using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        Socket server;
        Thread atender;
        public Form1()
        {
            InitializeComponent();
        }
        bool notificacion = true;
        private void Form1_Load(object sender, EventArgs e)
        {
        }

        private void button1_Click(object sender, EventArgs e)
        {
            notificacion = false;
            string mensaje = "1/" + nombre.Text + "/*/" + contraseña.Text;
            // Enviamos al servidor el nombre tecleado
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
            MessageBox.Show("Bienvenido " + mensaje + ".");
            ThreadStart ts = delegate { atender_mensajes_servidor(); };
            atender = new Thread(ts);
            atender.Start();
            notificacion = true;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            notificacion = false;
            if (Líder.Checked)
            {
                string mensaje = "3/";
                // Enviamos al servidor el nombre tecleado
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                //Recibimos la respuesta del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                MessageBox.Show("El Líder es: " + mensaje);
            }
            else if (Empates.Checked)
            {
                string mensaje = "4/";
                // Enviamos al servidor el nombre tecleado
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                //Recibimos la respuesta del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                MessageBox.Show("Han habido " + mensaje + " empates.");
            }
            else if (Palizas.Checked)
            {
                string mensaje = "5/";
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                //Recibimos la respuesta del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                MessageBox.Show("En " + mensaje + " partidos ha habido una diferencia de mas de 30 puntos.");
            }
            else
                MessageBox.Show("Seleccione una consulta.");
            notificacion = false;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            //Mensaje de desconexión
            string mensaje = "0/";
        
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            // Nos desconectamos
            this.BackColor = Color.Gray;
            server.Shutdown(SocketShutdown.Both);
            server.Close();
            atender.Abort();
        }



        private void button4_Click(object sender, EventArgs e)
        {
            IPAddress direc = IPAddress.Parse("147.83.117.22");
            IPEndPoint ipep = new IPEndPoint(direc, 50022);
            server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                server.Connect(ipep);//Intentamos conectar el socket
                this.BackColor = Color.Green;
                MessageBox.Show("Conectado");
            }
            catch (SocketException)
            {
                //Si hay excepcion imprimimos error y salimos del programa con return 
                MessageBox.Show("No he podido conectar con el servidor");
                return;
            }
        }

        public void Registrar(string mensaje)
        {
            notificacion = false;
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
            MessageBox.Show(mensaje);
            notificacion = true;
        }

        private void button5_Click(object sender, EventArgs e)
        {
            Form2 b = new Form2(this);
            b.Show();
        }

        private void atender_mensajes_servidor()
        {
            while (true)
            {
                if (notificacion == true)
                {
                    byte[] msg = new byte[80];
                    server.Receive(msg);
                    string mensaje = Encoding.ASCII.GetString(msg);
                    string[] codigo = mensaje.Split('/');
                    if (Convert.ToInt32(codigo[0]) == 10)
                    {
                        codigo[0] = null;
                        string listadeconectados = "Lista de conectados:\n";
                        foreach (string str in codigo)
                        {
                            listadeconectados = listadeconectados + str + "\n";
                        }
                        MessageBox.Show(listadeconectados);
                    }
                    if (Convert.ToInt32(codigo[0]) == 5)
                    {
                        string[] usuario = codigo[1].Split('\0');
                        Form3 partida = new Form3();
                        partida.pasar_informacion(this, usuario[0]);
                    }
                    if (Convert.ToInt32(codigo[0]) == 15)
                    {
                        MessageBox.Show("El jugador que te envió la solicitud se ha desconectado.");
                    }
                    if (Convert.ToInt32(codigo[0]) == 20)
                    {
                        if (codigo[1] == "1")
                        {
                            MessageBox.Show("El usuario " + codigo[2] + " ha aceptado tu solicitud de partida.");
                        }
                        if (codigo[1] == "0")
                        {
                            MessageBox.Show("El usuario " + codigo[2] + " no se encuentra o se ha desconectado.");
                        }
                        if (codigo[1] == "2")
                        {
                            MessageBox.Show("El usuario " + codigo[2] + " ha rechazado tu solicitud de partida.");
                        }
                    }
                }
            }
        }

        public void pasar_decision (int decision, string amfitrion)
    {
        notificacion = false;
        string enviar = "7/" + decision + "//" + amfitrion;
        byte[] msg1 = System.Text.Encoding.ASCII.GetBytes(enviar);
        server.Send(msg1);
        notificacion = true;
    }

        private void button6_Click(object sender, EventArgs e)
        {
            notificacion = false;
            string usuario = textBox1.Text;
            string mensaje = "6/" + usuario;
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
            notificacion = true;
        }
    }
}