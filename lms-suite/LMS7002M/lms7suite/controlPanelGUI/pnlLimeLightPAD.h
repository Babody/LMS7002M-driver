/**
@file 	pnlLimeLightPAD.h
@author Lime Microsystems
*/

#ifndef pnlLimeLightPAD_H
#define pnlLimeLightPAD_H
//(*Headers(pnlLimeLightPAD)
#include <wx/panel.h>
class wxCheckBox;
class wxNotebookEvent;
class wxComboBox;
class wxStaticLine;
class wxRadioBox;
class wxNotebook;
class wxStaticText;
class wxFlexGridSizer;
class wxButton;
class wxStaticBoxSizer;
//*)
#include <map>
#include "RegistersMap.h"
#include "GUIPanel.h"
class LMS7002_MainControl;
class ComboBoxMap;
class pnlLimeLightPAD : public wxPanel, GUIPanel
{
public:
    pnlLimeLightPAD(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, int style=0, wxString name="");
    void Initialize(LMS7002_MainControl *pControl);
    void UpdateGUI();
    virtual ~pnlLimeLightPAD();

//(*Declarations(pnlLimeLightPAD)
wxStaticText* StaticText10;
wxStaticText* StaticText22;
wxStaticText* StaticText9;
wxStaticText* StaticText20;
wxRadioBox* rgrMAC;
ComboBoxMap* cmbLML2_BIP;
wxStaticText* lblREV;
ComboBoxMap* cmbRXWRCLK_MUX;
ComboBoxMap* cmbLML2_TX_PST;
wxStaticText* StaticText29;
ComboBoxMap* cmbLML1_AQP;
wxNotebook* Notebook1;
wxStaticText* StaticText37;
wxCheckBox* chkTXEN_B;
wxCheckBox* chkSRST_RXFIFO;
wxStaticText* StaticText13;
wxStaticText* StaticText33;
wxStaticText* StaticText2;
wxCheckBox* chkFCLK1_INV;
wxCheckBox* chkTXDIVEN;
ComboBoxMap* cmbTXWRCLK_MUX;
wxCheckBox* chkFCLK1_PE;
wxStaticText* StaticText30;
wxStaticText* StaticText14;
ComboBoxMap* cmbRXRDCLK_MUX;
wxButton* btnReadVerRevMask;
wxCheckBox* chkMRST_RX_B;
ComboBoxMap* cmbLML2_BQP;
ComboBoxMap* cmbLML2_S1S;
wxCheckBox* chkMRST_TX_B;
wxRadioBox* rgrDIQDIR2;
wxRadioBox* rgrDIQDIRCTR1;
wxStaticText* StaticText26;
wxStaticText* StaticText6;
wxCheckBox* chkTXNRX1_PE;
wxCheckBox* chkMCLK2_PE;
wxCheckBox* chkSCL_PE;
ComboBoxMap* cmbLML1_S3S;
wxStaticText* StaticText40;
ComboBoxMap* cmbLML2_AIP;
wxCheckBox* chkFCLK2_INV;
ComboBoxMap* cmbMCLK2DLY;
wxStaticText* StaticText19;
wxStaticText* StaticText32;
ComboBoxMap* cmbLML1_BQP;
wxStaticText* StaticText8;
wxStaticText* StaticText38;
wxStaticText* StaticText11;
wxStaticText* StaticText18;
ComboBoxMap* cmbTXRDCLK_MUX;
ComboBoxMap* cmbLML1_S1S;
wxCheckBox* chkDIQ1_PE;
wxRadioBox* rgrENABLEDIRCTR2;
wxStaticText* StaticText31;
wxPanel* Panel1;
wxRadioBox* rgrSCL_DS;
wxCheckBox* chkLRST_TX_B;
wxStaticText* StaticText1;
wxCheckBox* chkIQ_SEL_EN_2_PE;
wxStaticText* StaticText27;
ComboBoxMap* cmbLML2_RX_PRE;
wxStaticText* StaticText3;
wxCheckBox* chkFCLK2_PE;
wxCheckBox* chkIQ_SEL_EN_1_PE;
wxRadioBox* rgrENABLEDIR1;
wxRadioBox* rgrSDIO_DS;
wxStaticText* StaticText21;
wxRadioBox* rgrENABLEDIR2;
wxStaticText* StaticText39;
wxCheckBox* chkMCLK1_PE;
wxRadioBox* rgrDIQDIR1;
wxStaticText* StaticText23;
wxStaticText* StaticText24;
wxRadioBox* rgrDIQ2_DS;
wxCheckBox* chkSRST_TXFIFO;
ComboBoxMap* cmbLML2_RX_PST;
wxCheckBox* chkTXNRX2_PE;
wxCheckBox* chkRXDIVEN;
wxCheckBox* chkMRST_TX_A;
wxCheckBox* chkDIQ2_PE;
ComboBoxMap* cmbMCLK1SRC;
ComboBoxMap* cmbLML1_AIP;
ComboBoxMap* cmbMIMO_SISO;
wxStaticText* StaticText5;
wxStaticText* StaticText34;
wxStaticText* StaticText7;
wxCheckBox* chkMOD_EN;
wxRadioBox* rgrSDA_DS;
wxCheckBox* chkTXEN_A;
wxCheckBox* chkLRST_RX_A;
wxRadioBox* rgrLML_MODE2;
wxCheckBox* chkRXEN_A;
wxCheckBox* chkMRST_RX_A;
wxRadioBox* rgrDIQDIRCTR2;
wxStaticLine* StaticLine1;
wxRadioBox* rgrLML_TXNRXIQ2;
ComboBoxMap* cmbLML2_S0S;
wxRadioBox* rgrENABLEDIRCTR1;
ComboBoxMap* cmbLML2_S3S;
wxCheckBox* chkSEN_PE;
wxStaticText* StaticText28;
wxCheckBox* chkTX_CLK_PE;
wxCheckBox* chkRXEN_B;
wxRadioBox* rgrLML_TXNRXIQ1;
wxStaticText* StaticText15;
wxRadioBox* rgrLML_FIDM2;
wxStaticText* StaticText12;
wxStaticText* StaticText35;
wxPanel* Panel2;
wxStaticText* lblVER;
wxStaticText* lblMASK;
wxRadioBox* rgrLML_FIDM1;
ComboBoxMap* cmbRXTSPCLKA_DIV;
wxCheckBox* chkSCLK_PE;
ComboBoxMap* cmbMCLK2SRC;
wxCheckBox* chkSDO_PE;
ComboBoxMap* cmbLML1_TX_PST;
wxStaticText* StaticText25;
ComboBoxMap* cmbLML2_S2S;
wxCheckBox* chkLRST_TX_A;
wxStaticText* StaticText17;
wxStaticText* StaticText4;
wxStaticText* StaticText36;
ComboBoxMap* cmbRX_MUX;
wxRadioBox* rgrSPIMODE;
wxRadioBox* rgrDIQ1_DS;
ComboBoxMap* cmbLML1_RX_PRE;
ComboBoxMap* cmbLML1_BIP;
wxCheckBox* chkSDIO_PE;
wxCheckBox* chkRX_CLK_PE;
wxRadioBox* rgrLML_MODE1;
ComboBoxMap* cmbLML1_TX_PRE;
ComboBoxMap* cmbLML2_AQP;
wxCheckBox* chkLRST_RX_B;
wxCheckBox* chkSDA_PE;
wxStaticText* StaticText16;
ComboBoxMap* cmbMCLK1DLY;
ComboBoxMap* cmbTXTSPCLKA_DIV;
ComboBoxMap* cmbLML1_S2S;
ComboBoxMap* cmbTX_MUX;
ComboBoxMap* cmbLML2_TX_PRE;
ComboBoxMap* cmbLML1_S0S;
ComboBoxMap* cmbLML1_RX_PST;
//*)

protected:
//(*Identifiers(pnlLimeLightPAD)
static const long ID_TX_CLK_PE;
static const long ID_RX_CLK_PE;
static const long ID_SDA_PE;
static const long ID_SCL_PE;
static const long ID_SDIO_PE;
static const long ID_SDO_PE;
static const long ID_SCLK_PE;
static const long ID_SEN_PE;
static const long ID_DIQ2_PE;
static const long ID_IQ_SEL_EN_2_PE;
static const long ID_TXNRX2_PE;
static const long ID_FCLK2_PE;
static const long ID_MCLK2_PE;
static const long ID_DIQ1_PE;
static const long ID_IQ_SEL_EN_1_PE;
static const long ID_TXNRX1_PE;
static const long ID_FCLK1_PE;
static const long ID_MCLK1_PE;
static const long ID_MOD_EN;
static const long ID_STATICLINE1;
static const long ID_LML_FIDM1;
static const long ID_LML_FIDM2;
static const long ID_LML_MODE1;
static const long ID_LML_MODE2;
static const long ID_LML_TXNRXIQ1;
static const long ID_LML_TXNRXIQ2;
static const long ID_LRST_TX_B;
static const long ID_MRST_TX_B;
static const long ID_LRST_TX_A;
static const long ID_MRST_TX_A;
static const long ID_LRST_RX_B;
static const long ID_MRST_RX_B;
static const long ID_LRST_RX_A;
static const long ID_MRST_RX_A;
static const long ID_SRST_RXFIFO;
static const long ID_SRST_TXFIFO;
static const long ID_SDA_DS;
static const long ID_SCL_DS;
static const long ID_SDIO_DS;
static const long ID_DIQ2_DS;
static const long ID_DIQ1_DS;
static const long ID_STATICTEXT33;
static const long ID_RX_MUX;
static const long ID_STATICTEXT34;
static const long ID_TX_MUX;
static const long ID_STATICTEXT35;
static const long ID_MIMO_SISO;
static const long ID_SPIMODE;
static const long ID_MAC;
static const long ID_RXEN_B;
static const long ID_RXEN_A;
static const long ID_TXEN_B;
static const long ID_TXEN_A;
static const long ID_STATICTEXT36;
static const long ID_VER;
static const long ID_STATICTEXT38;
static const long ID_REV;
static const long ID_STATICTEXT40;
static const long ID_MASK;
static const long ID_BUTTON1;
static const long ID_PANEL1;
static const long ID_STATICTEXT1;
static const long ID_LML1_S3S;
static const long ID_STATICTEXT2;
static const long ID_LML1_S2S;
static const long ID_STATICTEXT3;
static const long ID_LML1_S1S;
static const long ID_STATICTEXT4;
static const long ID_LML1_S0S;
static const long ID_STATICTEXT5;
static const long ID_LML1_BQP;
static const long ID_STATICTEXT6;
static const long ID_LML1_BIP;
static const long ID_STATICTEXT7;
static const long ID_LML1_AQP;
static const long ID_STATICTEXT8;
static const long ID_LML1_AIP;
static const long ID_STATICTEXT9;
static const long ID_LML2_S3S;
static const long ID_STATICTEXT10;
static const long ID_LML2_S2S;
static const long ID_STATICTEXT11;
static const long ID_LML2_S1S;
static const long ID_STATICTEXT12;
static const long ID_LML2_S0S;
static const long ID_STATICTEXT13;
static const long ID_LML2_BQP;
static const long ID_STATICTEXT14;
static const long ID_LML2_BIP;
static const long ID_STATICTEXT15;
static const long ID_LML2_AQP;
static const long ID_STATICTEXT16;
static const long ID_LML2_AIP;
static const long ID_STATICTEXT25;
static const long ID_TXRDCLK_MUX;
static const long ID_STATICTEXT26;
static const long ID_TXWRCLK_MUX;
static const long ID_STATICTEXT27;
static const long ID_RXRDCLK_MUX;
static const long ID_STATICTEXT28;
static const long ID_RXWRCLK_MUX;
static const long ID_STATICTEXT29;
static const long ID_MCLK2SRC;
static const long ID_STATICTEXT30;
static const long ID_MCLK1SRC;
static const long ID_STATICTEXT31;
static const long ID_TXTSPCLKA_DIV;
static const long ID_STATICTEXT32;
static const long ID_RXTSPCLKA_DIV;
static const long ID_TXDIVEN;
static const long ID_RXDIVEN;
static const long ID_FCLK1_INV;
static const long ID_FCLK2_INV;
static const long ID_STATICTEXT37;
static const long ID_MCLK1DLY;
static const long ID_STATICTEXT39;
static const long ID_MCLK2DLY;
static const long ID_DIQDIRCTR2;
static const long ID_DIQDIRCTR1;
static const long ID_DIQDIR2;
static const long ID_DIQDIR1;
static const long ID_ENABLEDIRCTR2;
static const long ID_ENABLEDIRCTR1;
static const long ID_ENABLEDIR2;
static const long ID_ENABLEDIR1;
static const long ID_STATICTEXT17;
static const long ID_LML1_TX_PST;
static const long ID_STATICTEXT18;
static const long ID_LML1_TX_PRE;
static const long ID_STATICTEXT19;
static const long ID_LML1_RX_PST;
static const long ID_STATICTEXT20;
static const long ID_LML1_RX_PRE;
static const long ID_STATICTEXT21;
static const long ID_LML2_TX_PST;
static const long ID_STATICTEXT22;
static const long ID_LML2_TX_PRE;
static const long ID_STATICTEXT23;
static const long ID_LML2_RX_PST;
static const long ID_STATICTEXT24;
static const long ID_LML2_RX_PRE;
static const long ID_PANEL2;
static const long ID_NOTEBOOK1;
//*)

private:
//(*Handlers(pnlLimeLightPAD)
    void ParameterChangeHandler(wxCommandEvent& event);
void OnbtnReadVerRevMaskClick(wxCommandEvent& event);
//*)

protected:
    void BuildContent(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size);
    void AssignEnumsToIds();
    DECLARE_EVENT_TABLE()
};
#endif