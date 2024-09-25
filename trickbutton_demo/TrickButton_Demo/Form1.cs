using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

using CodeProject.WinForms;

namespace TrickButtonDemo
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button QuitButton;
		private CodeProject.WinForms.MCTrickButton TrickButton;

		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Form1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.QuitButton = new System.Windows.Forms.Button();
			this.TrickButton = new CodeProject.WinForms.MCTrickButton();
			this.SuspendLayout();
			// 
			// QuitButton
			// 
			this.QuitButton.Location = new System.Drawing.Point(208, 128);
			this.QuitButton.Name = "QuitButton";
			this.QuitButton.TabIndex = 0;
			this.QuitButton.Text = "Quit";
			this.QuitButton.Click += new System.EventHandler(this.QuitButton_Click);
			// 
			// TrickButton
			// 
			this.TrickButton.Location = new System.Drawing.Point(100, 60);
			this.TrickButton.Name = "TrickButton";
			this.TrickButton.Text = "Click Me!";
			this.TrickButton.Click += new System.EventHandler(this.TrickButton_Click);
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(292, 165);
			this.Controls.AddRange(new System.Windows.Forms.Control[] { this.TrickButton,
																		this.QuitButton});
			this.Name = "Form1";
			this.Text = "Form1";
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new Form1());
		}

		private void QuitButton_Click(object sender, System.EventArgs e)
		{
			Close();	
		}

		private void TrickButton_Click(object sender, System.EventArgs e)
		{
			System.Windows.Forms.MessageBox.Show("You clicked me!");
		}
	}
}
