//
//  DNADrawPanel.cpp
//  DNAwx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "DNADrawPanel.h"
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <array>
#include <sstream>
#include <iomanip>
#include <cmath>

BEGIN_EVENT_TABLE(DNADrawPanel, wxPanel)
	EVT_PAINT(DNADrawPanel::PaintEvent)
END_EVENT_TABLE()

DNADrawPanel::DNADrawPanel(wxFrame* parent)
: wxPanel(parent)
{
	
}

void DNADrawPanel::PaintEvent(wxPaintEvent & evt)
{
	wxPaintDC dc(this);
	Render(dc);
}

void DNADrawPanel::PaintNow()
{
	wxClientDC dc(this);
	Render(dc);
}

void DNADrawPanel::SetHistoValues(const std::unordered_map<std::string, int>& histoValues)
{
	mHistogram = histoValues;
}

void DNADrawPanel::SetHeader(const std::string& header)
{
	mTitle = header;
}

void DNADrawPanel::Render(wxDC& dc)
{
	// Clear
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	
	//Draw histogram, if one has been generated
	if (mHistogram.size() > 0)
	{
		int max = 0;
		//determine total number of proteins coded for and max of single protein
		double total = 0;
		for (auto iter = mHistogram.begin(); iter != mHistogram.end(); ++iter)
		{
			if (iter->second > max)
			{
				max = iter->second;
			}
			total += iter->second;
		}
		double multiplier = 800 / max;

		//draw title
		wxSize size = dc.GetTextExtent(mTitle);
		dc.DrawText(mTitle, 512-(size.GetWidth()/2), 10);

		//instantiate variables
		int count = 0;
		int colorInd = 0;
		std::string catName = "";
		double percent = 0;

		//create brushes / pens
		wxColor *first = new wxColor(119, 136, 153);
		wxPen *firstPen = new wxPen(*first);
		wxBrush *firstBrush = new wxBrush(*first);
		wxColor *second = new wxColor(0, 206, 209);
		wxPen *secondPen = new wxPen(*second);
		wxBrush *secondBrush = new wxBrush(*second);

		//loop through unordered map
		for (auto iter = mHistogram.begin(); iter != mHistogram.end(); ++iter)
		{
			//concatenate info to print it
			catName += iter->first + ": " + std::to_string(iter->second);

			//calculate percentage
			percent = static_cast<double>(iter->second) / total;
			percent *= 100;
			std::string perc = std::to_string(percent);
			catName += " (" + perc + "%)";
			size = dc.GetTextExtent(catName);
			
			//alternate pen color
			if (dc.GetPen() == *secondPen)
			{
				dc.SetTextForeground(*first);
				dc.SetPen(*firstPen);
				dc.SetBrush(*firstBrush);
			}
			else
			{
				dc.SetTextForeground(*second);
				dc.SetPen(*secondPen);
				dc.SetBrush(*secondBrush);
			}
			
			//print category name and histogram bar
			dc.DrawText(catName, 160 - size.GetWidth(), 40 + count);
			dc.DrawRectangle(162, 40 + count, iter->second * multiplier, 18);
			count += 32;

			//reset variables
			catName = "";
			perc = "";
			percent = 0;
			colorInd++;
		}
	}
}
