//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Implementation of dialog to insert a move into a tree
//
// This file is part of Gambit
// Copyright (c) 2002, The Gambit Project
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif  // WX_PRECOMP
#include "game/efg.h"
#include "dlinsertmove.h"

//=========================================================================
//                   dialogInsertMove: Member functions
//=========================================================================

const int idMOVE_PLAYER_LISTBOX = 1000;
const int idMOVE_INFOSET_LISTBOX = 1001;

BEGIN_EVENT_TABLE(dialogInsertMove, wxDialog)
  EVT_LISTBOX(idMOVE_PLAYER_LISTBOX, dialogInsertMove::OnPlayer)
  EVT_LISTBOX(idMOVE_INFOSET_LISTBOX, dialogInsertMove::OnInfoset)
END_EVENT_TABLE()

dialogInsertMove::dialogInsertMove(wxWindow *p_parent, gbtEfgGame &p_efg)
  : wxDialog(p_parent, -1, "Insert Move"), m_efg(p_efg)
{
  m_playerItem = new wxListBox(this, idMOVE_PLAYER_LISTBOX);
  m_playerItem->Append("Chance");
  for (int pl = 1; pl <= m_efg.NumPlayers(); pl++) {
    m_playerItem->Append((char *)
			 (ToText(pl) + ": " + m_efg.GetPlayer(pl).GetLabel()));
  }
  m_playerItem->Append("New Player");
  m_playerItem->SetSelection(1);

  m_infosetItem = new wxListBox(this, idMOVE_INFOSET_LISTBOX);
  m_infosetItem->Append("New");
  gbtEfgPlayer player = p_efg.GetPlayer(1);
  for (int iset = 1; iset <= player.NumInfosets(); iset++) {
    m_infosetItem->Append((char *) (ToText(iset) + ": " +
				    player.GetInfoset(iset).GetLabel()));
  }
  m_infosetItem->SetSelection(0);

  wxBoxSizer *playerSizer = new wxBoxSizer(wxVERTICAL);
  playerSizer->Add(new wxStaticText(this, -1, "Player"),0, wxALL, 5);
  playerSizer->Add(m_playerItem, 0, wxEXPAND | wxALL, 5);

  wxBoxSizer *infosetSizer = new wxBoxSizer(wxVERTICAL);
  infosetSizer->Add(new wxStaticText(this, -1, "Information set"),
		    0, wxALL, 5);
  infosetSizer->Add(m_infosetItem, 0, wxEXPAND | wxALL, 5);

  wxBoxSizer *playerInfosetSizer = new wxBoxSizer(wxHORIZONTAL);
  playerInfosetSizer->Add(playerSizer, 1, wxALL, 5);
  playerInfosetSizer->Add(infosetSizer, 1, wxALL, 5);

  wxBoxSizer *actionSizer = new wxBoxSizer(wxHORIZONTAL);
  actionSizer->Add(new wxStaticText(this, -1, "Number of actions"),
		   0, wxALL, 5);
  m_actions = new wxSpinCtrl(this, -1, "2", wxDefaultPosition, wxDefaultSize,
			     wxSP_ARROW_KEYS, 1, 10000, 2);
  m_actions->Enable(m_infosetItem->GetSelection() == 0);
  actionSizer->Add(m_actions, 0, wxALL, 5);

  wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
  wxButton *okButton = new wxButton(this, wxID_OK, "OK");
  okButton->SetDefault();
  buttonSizer->Add(okButton, 0, wxALL, 5);
  buttonSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), 0, wxALL, 5);
  //  buttonSizer->Add(new wxButton(this, wxID_HELP, "Help"), 0, wxALL, 5);

  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(playerInfosetSizer, 0, wxALL | wxCENTER, 5);
  topSizer->Add(actionSizer, 0, wxALL | wxCENTER, 5);
  topSizer->Add(buttonSizer, 0, wxALL | wxCENTER, 5);
  
  SetAutoLayout(true);
  SetSizer(topSizer); 
  topSizer->Fit(this);
  topSizer->SetSizeHints(this); 
  Layout();
  CenterOnParent();
}

void dialogInsertMove::OnPlayer(wxCommandEvent &)
{
  int playerNumber = m_playerItem->GetSelection(); 

  gbtEfgPlayer player;
  if (playerNumber == 0)
    player = m_efg.GetChance();
  else if (playerNumber <= m_efg.NumPlayers())
    player = m_efg.GetPlayer(playerNumber);

  m_infosetItem->Clear();
  m_infosetItem->Append("New");
  if (!player.IsNull()) {
    for (int iset = 1; iset <= player.NumInfosets(); iset++) {
      m_infosetItem->Append((char *) (ToText(iset) + ": " +
			    player.GetInfoset(iset).GetLabel()));
    }
  }
  m_infosetItem->SetSelection(0);
  m_actions->SetValue(2);
  m_actions->Enable(true);
}

void dialogInsertMove::OnInfoset(wxCommandEvent &)
{
  int infosetNumber = m_infosetItem->GetSelection();

  if (infosetNumber > 0) {
    int playerNumber = m_playerItem->GetSelection();
    gbtEfgInfoset infoset;
    if (playerNumber == 0)
      infoset = m_efg.GetChance().GetInfoset(infosetNumber);
    else
      infoset = m_efg.GetPlayer(playerNumber).GetInfoset(infosetNumber);
    m_actions->Enable(false);
    m_actions->SetValue(infoset.NumActions());
  }
  else {
    m_actions->Enable(true);
    m_actions->SetValue(2);
  }
}

gbtEfgPlayer dialogInsertMove::GetPlayer(void) const
{
  int playerNumber = m_playerItem->GetSelection();

  if (playerNumber == 0) {
    return m_efg.GetChance();
  }
  else if (playerNumber <= m_efg.NumPlayers()) {
    return m_efg.GetPlayer(playerNumber);
  }
  else {
    gbtEfgPlayer player = m_efg.NewPlayer();
    player.SetLabel("Player " + ToText(m_efg.NumPlayers()));
    return player;
  }
}

gbtEfgInfoset dialogInsertMove::GetInfoset(void) const
{
  if (m_playerItem->GetSelection() <= m_efg.NumPlayers()) {
    gbtEfgPlayer player = GetPlayer();
    int infosetNumber = m_infosetItem->GetSelection();
    
    if (!player.IsNull() && infosetNumber > 0) {
      return player.GetInfoset(infosetNumber);
    }
    else {
      return 0;
    }
  }
  else {
    return 0;
  }
}

int dialogInsertMove::GetActions(void) const
{
  return (!GetInfoset().IsNull()) ? GetInfoset().NumActions() : m_actions->GetValue();
}

