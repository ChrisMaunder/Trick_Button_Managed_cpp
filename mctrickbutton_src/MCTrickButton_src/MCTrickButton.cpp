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
	// of the button. If it's within the critical distance then
	// we need to know from which direction the mouse is coming
	// from so we can move the button away from the mouse

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