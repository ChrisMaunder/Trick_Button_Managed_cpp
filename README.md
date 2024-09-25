# The MFC unclickable button ported to Managed C++

This article presents a port of the MFC Trick button to .NET using Managed C++ and Windows Forms




![](https://raw.githubusercontent.com/ChrisMaunder/trickbuttonmcpp/master/docs/assets/trickbuttonmcpp.GIF)

## Introduction

This article presents a port of the [MFC unclickable button](http://www.codeproject.com/buttonctrl/trick_button.asp) to .NET using Managed
C++ and Windows Forms. Nish did the original 1:1 port of the original MFC
control to Managed C++ to demonstrate how easy it was to port. Chris then took
this, rounded off some of the corners and wrapped the control up in an assembly
in order to demonstrate the class in use in a C# application.

## Porting to .NET

The `MCTrickButton` class is a fully managed Windows Form class derived
from `System::Windows::Forms::Button.`. Moving from MFC to Windows
Forms was relatively painless - and in some cases positively sweet. There were a
couple of issues though

As in the original we override the `
OnMouseMove` method. Initially an attempt 
was made to override `OnSetFocus`  as well, but 
the .NET `OnSetFocus` won't tell you what the previous control was which 
had the focus. Thus `WndProc` has been overridden and `WM_SETFOCUS` 
has been handled directly. The source code is commented and mostly self-explanatory.

There were also small
changes such as having the desktop window be a `Screen` object instead of a
`Control` (the analogy to Win32 being that the desktop is just another HWND)
meant a minor change to the code but apart from that it was smooth sailing

The control has been compiled to an assembly "CodeProject.WinForms"
(*CodeProject.WinForms.dll*)

## Issues

Our Managed C++ WinForms control does not work inside the VS.NET Forms designer.
I'm not sure if that's simply a case of C++ not having RAD support or if it's merely
a problem with our code. Suggestions welcome.

## TrickButton Source listing

### Header

```cpp
// MCTrickButton.h

#pragma once

using namespace System::Windows::Forms;

namespace CodeProject
{
    namespace WinForms
    {
        public __gc class MCTrickButton : public Button
        {
            // Constructor
        public:
            MCTrickButton();

            // Properties
        public:
            __property int get_JumpDistance()            
                                    { return m_nJumpDistance; }
            __property void set_JumpDistance(int value) 
                                    { m_nJumpDistance = value; }
        
            // Member variables
        protected:
            int m_nJumpDistance; // Pixels to jump

        protected:
            virtual void OnMouseMove(MouseEventArgs* e);
            virtual void WndProc(Message *pmsg);
        };
    }
}
```

### Implementation

```cpp
#include "StdAfx.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;

#include "MCTrickButton.h"

using namespace CodeProject::WinForms;

MCTrickButton::MCTrickButton()
{
     m_nJumpDistance = 5;
     TabStop = false;
}

// Called when the mouse moves over the button
void MCTrickButton::OnMouseMove(MouseEventArgs* e)
{
    // Get the current mouse position from the MouseEventArgs
    // object and convert to screen coords    
    Point point(e->X,e->Y);
    point = PointToScreen(point);

    // All pixels are used in parent client coordinates
    // So convert screen coords parent relative coords 
    Control *pParent = Parent;
    if (pParent != NULL)
        point = pParent->PointToClient(point);

    // Get the client area of the parent (so we know the bounds
    // of movement for the button
    Drawing::Rectangle ParentRect;
    if (pParent == NULL)
    {
        // use the current screen if no parent
        Screen *currentScreen = Screen::FromPoint(point);
        ParentRect = currentScreen->WorkingArea;
    }
    else
        ParentRect = pParent->ClientRectangle;

    // Get the dimensions of this button and convert to parent
    // coords
    Drawing::Rectangle ButtonRect = RectangleToScreen(ClientRectangle);
    if (pParent != NULL)
        ButtonRect = pParent->RectangleToClient(ButtonRect);

    Drawing::Point Center((ButtonRect.Right+ButtonRect.Left)/2,
                          (ButtonRect.Bottom+ButtonRect.Top)/2);

    // We now check where the mouse is relative to the center
    // of the button. We need to know from which direction the 
    // mouse is coming from so we can move the button away from 
    // the mouse

    Drawing::Rectangle NewButtonRect = ButtonRect;

    // mouse attack from the right side
    if (point.X  > Center.X)
    {
        if (ButtonRect.Left > ParentRect.Left + ButtonRect.Width + 
                             m_nJumpDistance)
        {
            NewButtonRect.X -= ButtonRect.Right - point.X + 
                                   m_nJumpDistance;
        }
        else
        {
            NewButtonRect.X += point.X - ButtonRect.Left + 
                                   m_nJumpDistance;
        }
    }
    // mouse attack from the left
    else if (point.X < Center.X) 
    {
        if (ButtonRect.Right < ParentRect.Right - ButtonRect.Width - 
                                   m_nJumpDistance)
        {
            NewButtonRect.X += point.X - ButtonRect.Left + 
                                   m_nJumpDistance;
        }
        else
        {
            NewButtonRect.X -= ButtonRect.Right - point.X + 
                                   m_nJumpDistance;
        }
    }
    // mouse attack from below
    if (point.Y > Center.Y)
    {
        if (ButtonRect.Top > ParentRect.Top + ButtonRect.Height + 
                                   m_nJumpDistance)
        {
            NewButtonRect.Y -= ButtonRect.Bottom - point.Y + 
                                   m_nJumpDistance;
        }
        else
        {
            NewButtonRect.Y += point.Y - ButtonRect.Top + 
                                   m_nJumpDistance;
        }
    }
    // attack from above
    else if (point.Y < Center.Y)
    {
        if (ButtonRect.Bottom < ParentRect.Bottom - ButtonRect.Height - 
                                   m_nJumpDistance)
        {
            NewButtonRect.Y += point.Y - ButtonRect.Top + 
                                   m_nJumpDistance;
        }
        else
        {
            NewButtonRect.Y -= ButtonRect.Bottom - point.Y + 
                                   m_nJumpDistance;
        }
    }

    // now we move the button to the new point
    Location = Point(NewButtonRect.X, NewButtonRect.Y);

    Button::OnMouseMove(e);
}

void MCTrickButton::WndProc(Message *pMsg)
{
     // Call the base class function first
    Button::WndProc(pMsg);

    // If the button is recieving the focus then pass
    // it back to the window that gave it the focus
    if (pMsg->Msg == WM_SETFOCUS)
    {
        Control *pOldWnd = FromHandle(pMsg->WParam); 
        if (pOldWnd != NULL) 
            pOldWnd->Focus(); //set focus to previous control
    } 
}
```

## A sample app

```cpp
using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

using CodeProject.WinForms;

namespace TrickButtonDemo
{
    /// Summary description for Form1.
    public class Form1 : System.Windows.Forms.Form
    {
        private System.Windows.Forms.Button QuitButton;
        private CodeProject.WinForms.MCTrickButton TrickButton;

        /// Required designer variable.
        private System.ComponentModel.Container components = null;

        public Form1()
        {
            InitializeComponent();
        }

        /// Clean up any resources being used.
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
            this.QuitButton.Click += 
                          new System.EventHandler(this.QuitButton_Click);
            // 
            // TrickButton
            // 
            this.TrickButton.Location = new System.Drawing.Point(100, 60);
            this.TrickButton.Name = "TrickButton";
            this.TrickButton.Text = "Click Me!";
            this.TrickButton.Click += 
                          new System.EventHandler(this.TrickButton_Click);
            // 
            // Form1
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(292, 165);
            this.Controls.AddRange(new System.Windows.Forms.Control[] { 
                                     this.TrickButton, this.QuitButton });
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
```
