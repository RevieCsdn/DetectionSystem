#ifndef __UIEVENT_H__
#define __UIEVENT_H__

#include <wx/wx.h>
#include <wx/event.h>
#include "../Util/preDefine.h"
#include "../Util/UtilObject.h"

//#include "../BO/SL_170.h"
using namespace  gf;

//*********************************************************************************************************//

class CommonlyEvent: public wxNotifyEvent
{
    private:
        int type;           // 0 for cancel
        bool finish_flag;   // if finished. for repeat(has event dlg), false:send to event btn, true: send to modal function

    public:
        void    SetType(int t) { type = t; }
        void    SetFinishFlag(bool flag) { finish_flag = flag; }
        int     GetType() const { return type; }
        bool    GetFinishFlag() const { return finish_flag;}

    public:
        CommonlyEvent(wxEventType commandType = wxEVT_NULL, int id = 0):
          wxNotifyEvent(commandType, id)
          {
              type = 0;
              finish_flag = false;
          }

          CommonlyEvent(const CommonlyEvent& event): wxNotifyEvent(event)
          {
              type = event.type;
              finish_flag = event.finish_flag;
          }

          virtual wxEvent* Clone() const
          {
              return new CommonlyEvent(*this);
          }
};

typedef void (wxEvtHandler::*CommonlyEventtFunction)(CommonlyEvent&);

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_COMMONLY, 804)
END_DECLARE_EVENT_TYPES()

#define EVT_COMMONLY(id, fn)\
    DECLARE_EVENT_TABLE_ENTRY(wxEVT_COMMONLY, id, -1,\
    (wxObjectEventFunction) (wxEventFunction) (CommonlyEventtFunction) & fn, (wxObject *) NULL ),

//*********************************************************************************************************//

class SuperDogEvent : public wxNotifyEvent
{
private:
	int type;           
public:
	void    SetType(int t) { type = t; }
	int     GetType() const { return type; }
public:
	SuperDogEvent(wxEventType commandType = wxEVT_NULL, int id = 0) :
		wxNotifyEvent(commandType, id) 
	{
		type = 0;
	}

	SuperDogEvent(const SuperDogEvent& event) : wxNotifyEvent(event) 
	{
		type = event.type;
	}

	virtual wxEvent* Clone() const {
		return new SuperDogEvent(*this);
	}
};

typedef void (wxEvtHandler::*SuperDogEventFunction)(SuperDogEvent&);

BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EVENT_TYPE(wxEVT_SUPER_DOG, 801);
END_DECLARE_EVENT_TYPES()

#define EVT_SUPERDOG(id, fn)\
	DECLARE_EVENT_TABLE_ENTRY(wxEVT_SUPER_DOG, id, -1,\
	(wxObjectEventFunction)(wxEventFunction)(SuperDogEventFunction) & fn, (wxObject *) NULL ),

//*********************************************************************************************************//

// class RefreshEvent : public wxNotifyEvent
// {
//     public:
//         RefreshEvent(wxEventType commandType = wxEVT_NULL, int id = 0):
//             wxNotifyEvent(commandType, id) {
//         }
// 
//         RefreshEvent(const RefreshEvent& event): wxNotifyEvent(event) {
//         }
// 
//         virtual wxEvent* Clone() const {
//             return new RefreshEvent(*this);
//         }
// };
// 
// typedef void (wxEvtHandler::*RefreshEventFunction)(RefreshEvent&);
// 
// BEGIN_DECLARE_EVENT_TYPES()
//     DECLARE_EVENT_TYPE(wxEVT_REFRESH_CHANGED, 801)
// END_DECLARE_EVENT_TYPES()
// 
// #define EVT_REFRESH_CHANGED(id, fn)\
//         DECLARE_EVENT_TABLE_ENTRY(wxEVT_REFRESH_CHANGED, id, -1,\
//         (wxObjectEventFunction) (wxEventFunction) (RefreshEventFunction) & fn, (wxObject *) NULL ),

//*********************************************************************************************************//

// class JogEvent : public wxNotifyEvent
// {
//     private:
//         JB_DIRECTION    m_dir;
//         bool            m_move;
// 
//     public:
//         void SetAction(JB_DIRECTION v_dir, bool v_move)
//         {
//             m_dir = v_dir;
//             m_move = v_move;
//         }
// 
//         JB_DIRECTION GetDirection() const { return m_dir; }
//         bool IsMove() const { return m_move; }
// 
//     public:
//         JogEvent(wxEventType commandType = wxEVT_NULL, int id = 0):
//                      wxNotifyEvent(commandType, id)
//         {
//             m_dir = JB_DIR_E;
//             m_move = false;
//         }
// 
//         JogEvent(const JogEvent& event): wxNotifyEvent(event)
//         {
//             m_dir = event.m_dir;
//             m_move = event.m_move;
//         }
// 
//         virtual wxEvent *Clone() const { return new JogEvent(*this); }
// };
// 
// typedef void (wxEvtHandler::*JogEventFunction)(JogEvent&);
// 
// /*!
//  * Font selector control events and macros for handling them
//  */
// 
// BEGIN_DECLARE_EVENT_TYPES()
//     DECLARE_EVENT_TYPE(wxEVT_JOG_CHANGED, 802)
// END_DECLARE_EVENT_TYPES()
// 
// #define EVT_JOG_CHANGED(id, fn)\
//         DECLARE_EVENT_TABLE_ENTRY(wxEVT_JOG_CHANGED, id, -1,\
//         (wxObjectEventFunction) (wxEventFunction) (JogEventFunction) & fn, (wxObject *) NULL ),
// 
// //*********************************************************************************************************//
// 
// class RightClickEvent : public wxNotifyEvent
// {
//     private:
//         double pos_x; // event position
//         double pos_y; // event position
// 
//     public:
//         // set event position
//         void SetPosition(double x, double y)
//         {
//             pos_x = x;
//             pos_y = y;
//         }
// 
//         // get event position
//         void GetPosition(double& x, double& y) const
//         {
//             x = pos_x;
//             y = pos_y;
//         }
// 
//     public:
//         RightClickEvent(wxEventType commandType = wxEVT_NULL, int id = 0):
//             wxNotifyEvent(commandType, id)
//         {
//             pos_x = 0;
//             pos_y = 0;
//         }
// 
//         RightClickEvent(const RightClickEvent& event): wxNotifyEvent(event)
//         {
//             pos_x = event.pos_x;
//             pos_y = event.pos_y;
//         }
// 
//         virtual wxEvent* Clone() const {
//             return new RightClickEvent(*this);
//         }
// };
// 
// typedef void (wxEvtHandler::*RightClickEventFunction)(RightClickEvent&);
// 
// /*!
//  * Font selector control events and macros for handling them
//  */
// 
// BEGIN_DECLARE_EVENT_TYPES()
//     DECLARE_EVENT_TYPE(wxEVT_RIGHTCLICK, 803)
// END_DECLARE_EVENT_TYPES()
// 
// #define EVT_RIGHTCLICK(id, fn)\
//         DECLARE_EVENT_TABLE_ENTRY(wxEVT_RIGHTCLICK, id, -1,\
//         (wxObjectEventFunction) (wxEventFunction) (RightClickEventFunction) & fn, (wxObject *) NULL ),
// 
// //*********************************************************************************************************//
// /*
//  event when doubleclick the cmp in componentlist  
// */
// class ClickLocateEvent : public wxNotifyEvent
// {
//     private:
//         Point2d point;
//         int     flag;
//         Element * el_ment;
// 
// 
//     public:
//         void SetPoint(Point2d pt) { point = pt; }
//         Point2d GetPoint() { return point; }
//         void SetFlag(int flg) { flag = flg; }
//         int GetFlag() { return flag; }
//         void SetElement( Element * elm){ el_ment = elm; }
//         Element * GetElement(){ return el_ment;}
// 
//     public:
//         ClickLocateEvent(wxEventType commandType = wxEVT_NULL, int id = 0):
//             wxNotifyEvent(commandType, id) {
//                 el_ment = NULL;
//                 flag = 0;
//                 point = Point2d(0,0);
//         }
// 
//         ClickLocateEvent(const ClickLocateEvent& event): wxNotifyEvent(event) {
//             point = event.point;
//             flag  = event.flag;
//             el_ment = event.el_ment;
// 
//         }
// 
//         virtual wxEvent* Clone() const {
//             return new ClickLocateEvent(*this);
//         }
// };
// 
// typedef void (wxEvtHandler::*ClickLocateEventFunction)(ClickLocateEvent&);
// 
// /*!
//  * Font selector control events and macros for handling them
//  */
// 
// BEGIN_DECLARE_EVENT_TYPES()
// DECLARE_EVENT_TYPE(wxCLICK_LOCATE, 804)
// END_DECLARE_EVENT_TYPES()
// 
// #define EVT_CLICK_LOCATE(id, fn)\
//         DECLARE_EVENT_TABLE_ENTRY(wxCLICK_LOCATE, id, -1,\
//         (wxObjectEventFunction) (wxEventFunction) (ClickLocateEventFunction) & fn, (wxObject *) NULL ),
// //***********************************************************************************************************
// 
// /*
// *  event when using quick key  in mainframe 
// */
// class FastKeyEvent : public wxNotifyEvent
// {
//     private:
//         int flag;
//         int keycode;
//         wxChar keychar;
// 
//     public:
//         int GetFlag() { return flag; }
//         void SetFlag(int flg) { flag = flg; }
//         int GetKeyCode() { return keycode; }
//         void SetKeyCode(int kcode) { keycode = kcode; }
//         wxChar  GetUnicodeKey() { return keychar; }
//         void   SetUnicodeKey(wxChar k_char) { keychar = k_char; }
// 
//     public:
//         FastKeyEvent(wxEventType commandType = wxEVT_NULL, int id = 0):
//             wxNotifyEvent(commandType, id) {
//         }
// 
//         FastKeyEvent(const FastKeyEvent& event): wxNotifyEvent(event) {
//             flag = event.flag;
//             keychar  =  event.keychar;
//             keycode  =  event.keycode;
//         }
// 
//         virtual wxEvent* Clone() const {
//             return new FastKeyEvent(*this);
//         }
// };
// 
// typedef void (wxEvtHandler::*FastKeyEventFunction)(FastKeyEvent&);
// 
// /*!
//  * Font selector control events and macros for handling them
//  */
// 
// BEGIN_DECLARE_EVENT_TYPES()
// DECLARE_EVENT_TYPE(wxEVT_FAST_KEY, 805)
// END_DECLARE_EVENT_TYPES()
// 
// 
// #define EVT_GET_FAST_KEYCODE(id, fn)\
//         DECLARE_EVENT_TABLE_ENTRY(wxEVT_FAST_KEY, id, -1,\
//         (wxObjectEventFunction) (wxEventFunction) (FastKeyEventFunction) & fn, (wxObject *) NULL ),
// //***********************************************************************************************************
// 
// /*
// *  event when using quick key  in mainframe 
// */
// class GetNextSnEvent : public wxNotifyEvent
// {
// 
//     public:
//         GetNextSnEvent(wxEventType commandType = wxEVT_NULL, int id = 0):
//             wxNotifyEvent(commandType, id) {
//         }
// 
//         GetNextSnEvent(const GetNextSnEvent& event): wxNotifyEvent(event) {
//         }
// 
//         virtual wxEvent* Clone() const {
//             return new GetNextSnEvent(*this);
//         }
// };
// 
// typedef void (wxEvtHandler::*GetNextSnEventFunction)(GetNextSnEvent&);
// 
// BEGIN_DECLARE_EVENT_TYPES()
// DECLARE_EVENT_TYPE(wxEVT_GETNEXTSN, 806)
// END_DECLARE_EVENT_TYPES()
// 
// #define EVT_GETNEXTSN(id, fn)\
//         DECLARE_EVENT_TABLE_ENTRY(wxEVT_GETNEXTSN, id, -1,\
//         (wxObjectEventFunction) (wxEventFunction) (GetNextSnEventFunction) & fn, (wxObject *) NULL ),
// //***********************************************************************************************************
// 
// 
// /*
// *  event for update inspect number
// */
// class ShowINumEvent : public wxNotifyEvent
// {
//     private:
//         int     m_num;
// 
//     public:
//         void SetINum(int num) { m_num = num; }
//         int  GetINum() { return m_num; }
// 
//     public:
//         ShowINumEvent(wxEventType commandType = wxEVT_NULL, int id = 0):
//           wxNotifyEvent(commandType, id) {
//           }
// 
//           ShowINumEvent(const ShowINumEvent& event): wxNotifyEvent(event) {
//               m_num = event.m_num;
//           }
// 
//           virtual wxEvent* Clone() const {
//               return new ShowINumEvent(*this);
//           }
// };
// 
// typedef void (wxEvtHandler::*ShowINumEventFunction)(ShowINumEvent&);
// 
// BEGIN_DECLARE_EVENT_TYPES()
//     DECLARE_EVENT_TYPE(wxEVT_SHOWINUM, 806)
// END_DECLARE_EVENT_TYPES()
// 
// #define EVT_SHOWINUM(id, fn)\
//     DECLARE_EVENT_TABLE_ENTRY(wxEVT_SHOWINUM, id, -1,\
//     (wxObjectEventFunction) (wxEventFunction) (ShowINumEventFunction) & fn, (wxObject *) NULL ),
// //***********************************************************************************************************
// 
// /*
//     *  event for update inspect number
//     */
// class ShowFiduEvent : public wxNotifyEvent
// {
// private:
//     int     m_flag;     // 0 tf, other br
//     wxImage m_img;
// 
// public:
//     void SetFiduType(int not_top) { m_flag = not_top; }
//     int  GetFiduType() { return m_flag; }
// 
//     wxImage GetImage() { return m_img; };
//     void    SetImage(wxImage img) { m_img = img.Copy(); }
// 
// public:
//     ShowFiduEvent(wxEventType commandType = wxEVT_NULL, int id = 0):
//       wxNotifyEvent(commandType, id) {
//       }
// 
//       ShowFiduEvent(const ShowFiduEvent& event): wxNotifyEvent(event) {
//           m_flag = event.m_flag;
//           m_img = event.m_img.Copy();
//       }
// 
//       virtual wxEvent* Clone() const {
//           return new ShowFiduEvent(*this);
//       }
// };
// 
// typedef void (wxEvtHandler::*ShowFiduEventFunction)(ShowFiduEvent&);
// 
// BEGIN_DECLARE_EVENT_TYPES()
//     DECLARE_EVENT_TYPE(wxEVT_SHOWFIDU, 806)
// END_DECLARE_EVENT_TYPES()
// 
// #define EVT_SHOWFIDU(id, fn)\
//     DECLARE_EVENT_TABLE_ENTRY(wxEVT_SHOWFIDU, id, -1,\
//     (wxObjectEventFunction) (wxEventFunction) (ShowFiduEventFunction) & fn, (wxObject *) NULL ),
// //***********************************************************************************************************
// 
// /*
//     *  event for verify button
//     */
// 
// class MButtonEvent : public wxNotifyEvent
// {
// private:
//     int     m_type;     // 0 tf, other br
// 
// public:
//     void SetType(int v_type) { m_type = v_type; }
//     int  GetType() { return m_type; }
// 
// public:
//     MButtonEvent(wxEventType commandType = wxEVT_NULL, int id = 0):
//       wxNotifyEvent(commandType, id) {
//       }
// 
//       MButtonEvent(const MButtonEvent& event): wxNotifyEvent(event) {
//           m_type = event.m_type;
//       }
// 
//       virtual wxEvent* Clone() const {
//           return new MButtonEvent(*this);
//       }
// };
// 
// typedef void (wxEvtHandler::*MButtonEventFunction)(MButtonEvent&);
// 
// BEGIN_DECLARE_EVENT_TYPES()
//     DECLARE_EVENT_TYPE(wxEVT_MBUTTONEVENT, 806)
// END_DECLARE_EVENT_TYPES()
// 
// #define EVT_MBUTTONEVENT(id, fn)\
//     DECLARE_EVENT_TABLE_ENTRY(wxEVT_MBUTTONEVENT, id, -1,\
//     (wxObjectEventFunction) (wxEventFunction) (MButtonEventFunction) & fn, (wxObject *) NULL ),
// //***********************************************************************************************************
// /*
//     *  event for click verify image
//     */
// 
class ImageClickEvent : public wxNotifyEvent
{
private:
    double  m_pos_x;
    double  m_pos_y;

public:
    void SetPosition(double x, double y)
    {
        m_pos_x = x;
        m_pos_y = y;
    }
    double  GetPosX() { return m_pos_x; }
    double  GetPosY() { return m_pos_y; }

public:
    ImageClickEvent(wxEventType commandType = wxEVT_NULL, int id = 0):
      wxNotifyEvent(commandType, id) {
      }

      ImageClickEvent(const ImageClickEvent& event): wxNotifyEvent(event)
      {
          m_pos_x = event.m_pos_x;
          m_pos_y = event.m_pos_y;
      }

      virtual wxEvent* Clone() const
      {
          return new ImageClickEvent(*this);
      }
};

typedef void (wxEvtHandler::*ImageClickEventFunction)(ImageClickEvent&);

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_IMAGECLICKEVENT, 806)
END_DECLARE_EVENT_TYPES()

#define EVT_IMAGECLICKEVENT(id, fn)\
    DECLARE_EVENT_TABLE_ENTRY(wxEVT_IMAGECLICKEVENT, id, -1,\
    (wxObjectEventFunction) (wxEventFunction) (ImageClickEventFunction) & fn, (wxObject *) NULL ),
    //***********************************************************************************************************
#endif // __UIEVENT_H__
