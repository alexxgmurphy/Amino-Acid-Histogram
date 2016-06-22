//
//  DNADrawPanel.h
//  DNAwx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <string>
#include <memory>
#include <unordered_map>

class DNADrawPanel : public wxPanel
{
public:
	DNADrawPanel(wxFrame* parent);
	void PaintNow();
	void SetHistoValues(const std::unordered_map<std::string, int>& histoValues);
	void SetHeader(const std::string& header);

protected:
	void PaintEvent(wxPaintEvent & evt);
	void Render(wxDC& dc);
 
	DECLARE_EVENT_TABLE()
	
public:
	std::string mTitle;
	std::unordered_map<std::string, int> mHistogram;
};

