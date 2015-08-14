// ProgressDialog2.h

#ifndef __PROGRESS_DIALOG_2_H
#define __PROGRESS_DIALOG_2_H

#include "../../../Common/MyCom.h"

#include "../../../Windows/ErrorMsg.h"
#include "../../../Windows/Synchronization.h"
#include "../../../Windows/Thread.h"

#include "../../../Windows/Control/Dialog.h"
#include "../../../Windows/Control/ListView.h"
#include "../../../Windows/Control/ProgressBar.h"

#include "MyWindowsNew.h"

struct CProgressMessageBoxPair
{
  UString Title;
  UString Message;
};

struct CProgressFinalMessage
{
  CProgressMessageBoxPair ErrorMessage;
  CProgressMessageBoxPair OkMessage;

  bool ThereIsMessage() const { return !ErrorMessage.Message.IsEmpty() || !OkMessage.Message.IsEmpty(); }
};

class CProgressSync
{
  bool _stopped;
  bool _paused;

public:
  bool _bytesProgressMode;
  UInt64 _totalBytes;
  UInt64 _completedBytes;
  UInt64 _totalFiles;
  UInt64 _curFiles;
  UInt64 _inSize;
  UInt64 _outSize;

  UString _titleFileName;
  UString _status;
  UString _filePath;
  bool _isDir;

  UStringVector Messages;
  CProgressFinalMessage FinalMessage;

  NWindows::NSynchronization::CCriticalSection _cs;

  CProgressSync();

  bool Get_Stopped()
  {
    NWindows::NSynchronization::CCriticalSectionLock lock(_cs);
    return _stopped;
  }
  void Set_Stopped(bool val)
  {
    NWindows::NSynchronization::CCriticalSectionLock lock(_cs);
    _stopped = val;
  }

  bool Get_Paused();
  void Set_Paused(bool val)
  {
    NWindows::NSynchronization::CCriticalSectionLock lock(_cs);
    _paused = val;
  }

  void Set_BytesProgressMode(bool bytesProgressMode)
  {
    NWindows::NSynchronization::CCriticalSectionLock lock(_cs);
    _bytesProgressMode = bytesProgressMode;
  }

  HRESULT CheckStop();
  HRESULT ScanProgress(UInt64 numFiles, UInt64 totalSize, const UString &fileName, bool isDir = false);

  void Set_NumFilesTotal(UInt64 val);
  void Set_NumBytesTotal(UInt64 val);
  void Set_NumFilesCur(UInt64 val);
  HRESULT Set_NumBytesCur(const UInt64 *val);
  HRESULT Set_NumBytesCur(UInt64 val);
  void Set_Ratio(const UInt64 *inSize, const UInt64 *outSize);

  void Set_TitleFileName(const UString &fileName);
  void Set_Status(const UString &s);
  void Set_FilePath(const UString &path, bool isDir = false);

  void AddError_Message(const wchar_t *message);
  void AddError_Message_Name(const wchar_t *message, const wchar_t *name);
  void AddError_Code_Name(DWORD systemError, const wchar_t *name);

  bool ThereIsMessage() const { return !Messages.IsEmpty() || FinalMessage.ThereIsMessage(); }
};

class CProgressDialog: public NWindows::NControl::CModalDialog
{
  UString _titleFileName;
  UString _filePath;
  UString _status;
  bool _isDir;

  UString _background_String;
  UString _backgrounded_String;
  UString _foreground_String;
  UString _pause_String;
  UString _continue_String;
  UString _paused_String;

  int _buttonSizeX;
  int _buttonSizeY;

  UINT_PTR _timer;

  UString _title;

  class CU64ToI32Converter
  {
    unsigned _numShiftBits;
    UInt64 _range;
  public:
    CU64ToI32Converter(): _numShiftBits(0), _range(1) {}
    void Init(UInt64 range)
    {
      _range = range;
      // Windows CE doesn't like big number for ProgressBar.
      for (_numShiftBits = 0; range >= ((UInt32)1 << 15); _numShiftBits++)
        range >>= 1;
    }
    int Count(UInt64 val)
    {
      int res = (int)(val >> _numShiftBits);
      if (val == _range)
        res++;
      return res;
    }
  };

  CU64ToI32Converter _progressConv;
  UInt64 _progressBar_Pos;
  UInt64 _progressBar_Range;

  NWindows::NControl::CProgressBar m_ProgressBar;
  // FIXME NWindows::NControl::CListView _messageList;

  int _numMessages;

  // FIXME CMyComPtr<ITaskbarList3> _taskbarList;
  // FIXME HWND _hwndForTaskbar;

  UInt32 _prevTime;
  UInt64 _elapsedTime;

  UInt64 _prevPercentValue;
  UInt64 _prevElapsedSec;
  UInt64 _prevRemainingSec;

  UInt64 _totalBytes_Prev;
  UInt64 _processed_Prev;
  UInt64 _packed_Prev;
  UInt64 _ratio_Prev;
  UString _filesStr_Prev;

  unsigned _prevSpeed_MoveBits;
  UInt64 _prevSpeed;

  bool _foreground;

  unsigned _numReduceSymbols;

  bool _wasCreated;
  bool _needClose;

  unsigned _numPostedMessages;
  UInt32 _numAutoSizeMessages;

  bool _errorsWereDisplayed;

  bool _waitCloseByCancelButton;
  bool _cancelWasPressed;

  bool _inCancelMessageBox;
  bool _externalCloseMessageWasReceived;


  #ifdef __ITaskbarList3_INTERFACE_DEFINED__
  void SetTaskbarProgressState(TBPFLAG tbpFlags)
  {
    if (_taskbarList && _hwndForTaskbar)
      _taskbarList->SetProgressState(_hwndForTaskbar, tbpFlags);
  }
  #endif
  void SetTaskbarProgressState();

  void UpdateStatInfo(bool showAll);
  bool OnTimer(WPARAM timerID, LPARAM callback);
  void SetProgressRange(UInt64 range);
  void SetProgressPos(UInt64 pos);
  virtual bool OnInit();
  virtual bool OnSize(WPARAM wParam, int xSize, int ySize);
  virtual void OnCancel();
  virtual void OnOK();
  NWindows::NSynchronization::CManualResetEvent _createDialogEvent;
  NWindows::NSynchronization::CManualResetEvent _dialogCreatedEvent;
  #ifndef _SFX
  void AddToTitle(LPCWSTR string);
  #endif

  void SetPauseText();
  void SetPriorityText();
  void OnPauseButton();
  void OnPriorityButton();
  bool OnButtonClicked(int buttonID, HWND buttonHWND);
  bool OnMessage(UINT message, WPARAM wParam, LPARAM lParam);

  void SetTitleText();
  void ShowSize(int id, UInt64 val, UInt64 &prev);

  void UpdateMessagesDialog();

  void AddMessageDirect(LPCWSTR message, bool needNumber);
  void AddMessage(LPCWSTR message);

  bool OnExternalCloseMessage();
  void EnableErrorsControls(bool enable);

  void ShowAfterMessages(HWND wndParent);

  void CheckNeedClose();
public:
  CProgressSync Sync;
  bool CompressingMode;
  // FIXME - not supported bool WaitMode;
  bool ShowCompressionInfo;
  bool MessagesDisplayed; // = true if user pressed OK on all messages or there are no messages.
  int IconID;

  HWND MainWindow;
  #ifndef _SFX
  UString MainTitle;
  UString MainAddTitle;
  ~CProgressDialog();
  #endif

  CProgressDialog();
  void WaitCreating()
  {
    _createDialogEvent.Set();
    _dialogCreatedEvent.Lock();
  }

  INT_PTR Create(const UString &title, NWindows::CThread &thread, HWND wndParent = 0);

  void ProcessWasFinished();
};


class CProgressCloser
{
  CProgressDialog *_p;
public:
  CProgressCloser(CProgressDialog &p) : _p(&p) {}
  ~CProgressCloser() { _p->ProcessWasFinished(); }
};

class CProgressThreadVirt
{
  FString ErrorPath1;
  FString ErrorPath2;
protected:
  CProgressFinalMessage FinalMessage;

  // error if any of HRESULT, ErrorMessage, ErrorPath
  virtual HRESULT ProcessVirt() = 0;
  void Process();
public:
  HRESULT Result;
  bool ThreadFinishedOK; // if there is no fatal exception
  CProgressDialog ProgressDialog;

  static THREAD_FUNC_DECL MyThreadFunction(void *param)
  {
    CProgressThreadVirt *p = (CProgressThreadVirt *)param;
    try
    {
      p->Process();
      p->ThreadFinishedOK = true;
    }
    catch (...) { p->Result = E_FAIL; }
    return 0;
  }

  void SetErrorPath1(const FString &path) { ErrorPath1 = path; }
  void SetErrorPath2(const FString &path) { ErrorPath2 = path; }

  HRESULT Create(const UString &title, HWND parentWindow = 0);
  CProgressThreadVirt(): Result(E_FAIL), ThreadFinishedOK(false) {}

  CProgressMessageBoxPair &GetMessagePair(bool isError) { return isError ? FinalMessage.ErrorMessage : FinalMessage.OkMessage; }

};

UString HResultToMessage(HRESULT errorCode);

#endif