// Mainframe macro generated from application: /usr/local/root/bin/root.exe
// By ROOT version 5.14/00d on 2007-11-22 09:05:39

#if !defined( __CINT__) || defined (__MAKECINT__)

#ifndef ROOT_TGDockableFrame
#include "TGDockableFrame.h"
#endif
#ifndef ROOT_TGDoubleSlider
#include "TGDoubleSlider.h"
#endif
#ifndef ROOT_TGMdiDecorFrame
#include "TGMdiDecorFrame.h"
#endif
#ifndef ROOT_TG3DLine
#include "TG3DLine.h"
#endif
#ifndef ROOT_TGMdiFrame
#include "TGMdiFrame.h"
#endif
#ifndef ROOT_TGMdiMainFrame
#include "TGMdiMainFrame.h"
#endif
#ifndef ROOT_TGuiBldHintsButton
#include "TGuiBldHintsButton.h"
#endif
#ifndef ROOT_TGButtonGroup
#include "TGButtonGroup.h"
#endif
#ifndef ROOT_TGMdiMenu
#include "TGMdiMenu.h"
#endif
#ifndef ROOT_TGListBox
#include "TGListBox.h"
#endif
#ifndef ROOT_TGNumberEntry
#include "TGNumberEntry.h"
#endif
#ifndef ROOT_TGScrollBar
#include "TGScrollBar.h"
#endif
#ifndef ROOT_TGuiBldHintsEditor
#include "TGuiBldHintsEditor.h"
#endif
#ifndef ROOT_TRootBrowser
#include "TRootBrowser.h"
#endif
#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif
#ifndef ROOT_TGMenu
#include "TGMenu.h"
#endif
#ifndef ROOT_TGFileDialog
#include "TGFileDialog.h"
#endif
#ifndef ROOT_TGShutter
#include "TGShutter.h"
#endif
#ifndef ROOT_TRootDialog
#include "TRootDialog.h"
#endif
#ifndef ROOT_TGCanvas
#include "TGCanvas.h"
#endif
#ifndef ROOT_TGFSContainer
#include "TGFSContainer.h"
#endif
#ifndef ROOT_TGButton
#include "TGButton.h"
#endif
#ifndef ROOT_TGuiBldEditor
#include "TGuiBldEditor.h"
#endif
#ifndef ROOT_TRootContextMenu
#include "TRootContextMenu.h"
#endif
#ifndef ROOT_TGFSComboBox
#include "TGFSComboBox.h"
#endif
#ifndef ROOT_TFitEditor
#include "TFitEditor.h"
#endif
#ifndef ROOT_TGLabel
#include "TGLabel.h"
#endif
#ifndef ROOT_TRootGuiBuilder
#include "TRootGuiBuilder.h"
#endif
#ifndef ROOT_TGTab
#include "TGTab.h"
#endif
#ifndef ROOT_TGListView
#include "TGListView.h"
#endif
#ifndef ROOT_TGSplitter
#include "TGSplitter.h"
#endif
#ifndef ROOT_TRootCanvas
#include "TRootCanvas.h"
#endif
#ifndef ROOT_TGStatusBar
#include "TGStatusBar.h"
#endif
#ifndef ROOT_TGListTree
#include "TGListTree.h"
#endif
#ifndef ROOT_TGToolTip
#include "TGToolTip.h"
#endif
#ifndef ROOT_TGToolBar
#include "TGToolBar.h"
#endif
#ifndef ROOT_TRootEmbeddedCanvas
#include "TRootEmbeddedCanvas.h"
#endif
#ifndef ROOT_TCanvas
#include "TCanvas.h"
#endif
#ifndef ROOT_TGuiBldDragManager
#include "TGuiBldDragManager.h"
#endif

#include "Riostream.h"

#endif

void viewData()
{

   // main frame
   TGMainFrame *fMainFrame3637 = new TGMainFrame(gClient->GetRoot(),10,10,kMainFrame | kVerticalFrame);
   fMainFrame3637->SetLayoutBroken(kTRUE);

   // embedded canvas
   TRootEmbeddedCanvas *fRootEmbeddedCanvas3471 = new TRootEmbeddedCanvas(0,fMainFrame3637,152,128);
   Int_t wfRootEmbeddedCanvas3471 = fRootEmbeddedCanvas3471->GetCanvasWindowId();
   TCanvas *c123 = new TCanvas("c123", 10, 10, wfRootEmbeddedCanvas3471);
   fRootEmbeddedCanvas3471->AdoptCanvas(c123);
   fMainFrame3637->AddFrame(fRootEmbeddedCanvas3471, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fRootEmbeddedCanvas3471->MoveResize(280,8,152,128);

   // embedded canvas
   TRootEmbeddedCanvas *fRootEmbeddedCanvas3485 = new TRootEmbeddedCanvas(0,fMainFrame3637,152,136);
   Int_t wfRootEmbeddedCanvas3485 = fRootEmbeddedCanvas3485->GetCanvasWindowId();
   TCanvas *c124 = new TCanvas("c124", 10, 10, wfRootEmbeddedCanvas3485);
   fRootEmbeddedCanvas3485->AdoptCanvas(c124);
   fMainFrame3637->AddFrame(fRootEmbeddedCanvas3485, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fRootEmbeddedCanvas3485->MoveResize(280,184,152,136);
   TGTextButton *fTextButton3531 = new TGTextButton(fMainFrame3637,"fTextButton3531");
   fTextButton3531->SetTextJustify(36);
   fTextButton3531->Resize(99,24);
   fMainFrame3637->AddFrame(fTextButton3531, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButton3531->MoveResize(56,312,99,24);

   fMainFrame3637->MapSubwindows();

   fMainFrame3637->Resize(fMainFrame3637->GetDefaultSize());
   fMainFrame3637->MapWindow();
   fMainFrame3637->Resize(490,372);
}  
