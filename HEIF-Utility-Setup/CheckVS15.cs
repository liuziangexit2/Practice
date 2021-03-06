﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace HEIF_Utility_Setup
{
    public partial class CheckVS15 : UserControl
    {
        public delegate void NavHandler(object sender, NavArgs e);
        public event NavHandler OnNav;

        public CheckVS15()
        {
            InitializeComponent();
        }

        [DllImport("HUD.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        private unsafe extern static void test_dll(byte* buffer);

        private static unsafe bool invoke_test_dll()
        {
            try
            {
                var output_buffer = new byte[5];
                fixed (byte* input = &output_buffer[0])
                {
                    test_dll(input);
                }
                return Encoding.Default.GetString(output_buffer, 0, 5) == "naive";
            }
            catch (Exception)
            {
                return false;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            button1.Enabled = false;

            textBox1.Text += "\r\n\r\n";

            var isok = invoke_test_dll();

            if (isok)
                textBox1.Text += "测试程序 ---- 通过\r\n";
            else
                textBox1.Text += "测试程序 ---- 未通过\r\n";

            textBox1.Text += "\r\n";

            if (isok)
            {
                textBox1.Text += "运行环境符合要求，点击底部的“下一步”来继续。\r\n";
                OnNav(this, new NavArgs() { type = 3 });
                OnNav(this, new NavArgs() { type = 4 });
            }
            else
            {
                textBox1.Text += "运行环境不符合要求，HEIF 实用工具 无法在此计算机正常工作，请退出配置向导。\r\n\r\n请在 https://liuziangexit.com/HEIF-Utility/Help/No10.html 解决此问题。";
            }

        }

        private void CheckVS15_Load(object sender, EventArgs e)
        {
            OnNav(this, new NavArgs() { type = 1 });
            //OnNav(this, new NavArgs() { type = 2 });
        }
    }
}
