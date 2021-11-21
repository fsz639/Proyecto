using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public partial class Form3 : Form
    {
        string a;
        Form1 el_otro_form;
        int decision = 0;
        public Form3()
        {
            InitializeComponent();
        }

        public void pasar_informacion(Form1 form1, string usuario)
        {
            el_otro_form = form1;
            a = usuario;
            textBox1.Text = "El jugador " + a + " te ha retado a una partida, aceptas la invitacion?";
            this.ShowDialog();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            decision = 1;
            this.Hide();
            el_otro_form.pasar_decision(decision, a);
            this.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            decision = 2;
            this.Hide();
            el_otro_form.pasar_decision(decision, a);
            this.Close();
        }
    }
}
