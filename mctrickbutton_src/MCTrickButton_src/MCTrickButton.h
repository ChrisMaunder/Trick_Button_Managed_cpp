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
			__property int get_JumpDistance()			{ return m_nJumpDistance; }
			__property void set_JumpDistance(int value) { m_nJumpDistance = value; }

		
			// Member variables
		protected:
			int m_nJumpDistance; // Pixels to jump

		protected:
			virtual void OnMouseMove(MouseEventArgs* e);
			virtual void WndProc(Message *pmsg);
		};
	}
}