#include "../headers/mainFrame.h"

MyFrame::MyFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title){
    init();
}

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_TEXT_ENTER(wxID_ANY, MyFrame::OnTextEnter)
wxEND_EVENT_TABLE()

void MyFrame::init() {

    // Creating Sizers
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
    bottomSizer = new wxBoxSizer(wxVERTICAL);

    // Creating Buttons etc. for Top Sizer
    inputBar = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    wxButton* inputButton = new wxButton(this, wxID_ANY, "Search");

    wxArrayString choices;
    choices.Add("Title"); choices.Add("Author"); choices.Add("Genre");
    choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices);
    choice->Select(0);

    // Creating buttons for Bottom Sizer and scroled window
    scrolledPanel = new wxScrolled<wxPanel>(this, wxID_ANY);
    scrolledPanel->SetScrollbars(20,20,50,50);
    scrolledSizer = new wxBoxSizer(wxVERTICAL);

    scrolledPanel->SetSizer(scrolledSizer);
    scrolledPanel->FitInside();

    // Assigning to Sizers
    topSizer->Add(inputBar, 4, wxALL, 5);
    topSizer->Add(choice, 2, wxALL, 5);
    topSizer->Add(inputButton, 1, wxALL, 5);

    // Add the scrolled panel to the bottomSizer
    bottomSizer->Add(scrolledPanel, 1, wxEXPAND);

    mainSizer->Add(topSizer, 1, wxEXPAND | wxALL, 2);
    mainSizer->Add(bottomSizer, 10, wxEXPAND | wxALL, 2);

    this->SetSizerAndFit(mainSizer);


    // Bind the button click event to the OnTextEnter function
    inputButton->Bind(wxEVT_BUTTON, &MyFrame::OnTextEnter, this);
}

void MyFrame::OnTextEnter(wxCommandEvent& event)
{
    search = inputBar->GetValue();
    int category = choice->GetSelection();

    // Clear existing buttons from scrolledSizer
    scrolledSizer->Clear(true);


    // Explicit type change
    choiceOfSearch cat;
    switch(category)
    {
        case 0:
            cat = choiceOfSearch::Book;
            break;
        case 1:
            cat = choiceOfSearch::Author;
            break;
        case 2:
            cat = choiceOfSearch::Index;
            break;
        default:
            cat = choiceOfSearch::Book;
            break;
    }


    // Database
    db->setSearch(search,cat);
    std::vector<BookInfo> results = db->printSelectedColumns();


   if (!search.empty())
    {
        int i=0;
        for(const auto& result : results) {

            // Add a horizontal line between images (except for the first one)
            if (i > 0) {
            wxStaticLine* horizontalLine = new wxStaticLine(scrolledPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
            scrolledSizer->Add(horizontalLine, 1, wxEXPAND | wxLEFT | wxRIGHT, 10); // Adjust margins as needed
            }

            // Creating Sizers
            wxBoxSizer* resultSizer = new wxBoxSizer(wxHORIZONTAL);
            wxBoxSizer* noteSizer = new wxBoxSizer(wxVERTICAL);

            // Getting image and Creating bitmap to print out
            wxBitmap bitmap("../images/"+result.graphic+".png", wxBITMAP_TYPE_PNG);
            wxStaticBitmap* staticBitmap = new wxStaticBitmap(scrolledPanel, wxID_ANY, bitmap);

            // Static text to print out on the right of image
            wxStaticText* title = new wxStaticText(scrolledPanel, wxID_ANY, "Title: " + result.title);
            wxStaticText* author = new wxStaticText(scrolledPanel, wxID_ANY, "Author: " + result.author);
            wxStaticText* genre = new wxStaticText(scrolledPanel, wxID_ANY, "Genre: " + result.ind);

            bool rented = result.isRented;

            wxStaticText* rentedText = (rented) ? 
            new wxStaticText(scrolledPanel, wxID_ANY,"Rented") :
            new wxStaticText(scrolledPanel, wxID_ANY,"For rent");

            std::string date = (rented) ? result.date : " ";
            wxStaticText* dateText = new wxStaticText(scrolledPanel, wxID_ANY,date);


            // Button to rent books with Lambda function
            wxButton* buttonRent = new wxButton(scrolledPanel,wxID_ANY,"Rent");

            buttonRent->Bind(wxEVT_BUTTON, [this, &result](wxCommandEvent& event) {
                int resultId = result.id;
                std::cout << resultId << '\n';

                if (!result.isRented && db) {
                    db->rentBook(resultId);
                }

                event.Skip();

                return;
            });

            // Adding elements to the Sizer (in order)
            noteSizer->Add(title, 0, wxALL, 3);
            noteSizer->Add(author, 0, wxALL, 3);
            noteSizer->Add(genre, 0, wxALL, 3); 
            noteSizer->Add(rentedText, 0, wxALL, 3);
            noteSizer->Add(dateText,0 ,wxALIGN_BOTTOM | wxTOP,100);

            resultSizer->Add(staticBitmap, 0, wxALIGN_LEFT);
            resultSizer->Add(noteSizer, 1, wxEXPAND);
            resultSizer->Add(buttonRent, 0, wxALIGN_BOTTOM | wxALIGN_RIGHT);

            scrolledSizer->Add(resultSizer, 0, wxEXPAND | wxALL ,7);

            ++i;
        }
    }

    // Refresh the layout
    scrolledSizer->Layout();
    scrolledPanel->FitInside();

    inputBar->SetFocus();
}