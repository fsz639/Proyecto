using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WindowsFormsApplication1;

namespace WindowsFormsApplication1
{
    public partial class Form2 : Form
    {
        public Form1 A;
        public Form2(Form1 a)
        {
            A = a;
            InitializeComponent();
        }

        private void Form2_Load(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (textBox2.Text == textBox3.Text)
            {
                string mensaje = "2/" + textBox1.Text + "/*/" + textBox2.Text;
                A.Registrar(mensaje);
            }
            else
                MessageBox.Show("Las contraseñas no coinciden.");
        }
    }
}
