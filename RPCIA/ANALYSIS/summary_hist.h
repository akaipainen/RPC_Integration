#if !defined(SUMMARY_HIST_H)
#define SUMMARY_HIST_H

#include <iostream>

#include <TString.h>
#include <TH1.h>
#include <TCanvas.h>

template <class H>
class SummaryHist
{
private:
    std::vector<H*> tdc_;

    const char* name_;

    int ntdcs_;
    bool separate_;
    
public:
    // Constructor
    SummaryHist(const char* name)
     : name_(name)
     , ntdcs_(0)
     , separate_(false)
    {
    }

    // Destructor
    ~SummaryHist()
    {
        // Delete all histograms
        for (auto it = tdc_.begin(); it != tdc_.end(); it++)
        {
            delete *it;
        }
    }

    // Copy constructor
    SummaryHist(const SummaryHist<H>& other)
     : ntdcs_(other.ntdcs_)
     , separate_(other.separate_)
    {
        for (auto it = other.tdc_.begin(); it != other.tdc_.end(); it++)
        {
            tdc_.push_back(new H(**it));
        }
    }

    // Copy assignment operator
    SummaryHist<H>& operator=(const SummaryHist<H>& other)
    {
        auto temp(other);
        std::swap(ntdcs_, temp.ntdcs_);
        std::swap(tdc_, temp.tdc_);
        std::swap(separate_, temp.separate_);

        return *this;
    }

    // Move constructor
    SummaryHist(SummaryHist<H> &&other)
     : ntdcs_(other.ntdcs_)
    {
        tdc_ = other.tdc_;
        separate_ = other.separate_;

        other.tdc_.clear();
    }

    // Move assignment operator
    SummaryHist<H>& operator=(SummaryHist<H> &&other)
    {
        ntdcs_ = other.ntdcs_;
        tdc_ = std::move(other.tdc_);
        separate_ = std::move(other.separate_);
        return *this;
    }

    template <class ...ARGS>
    void init(int ntdcs, ARGS && ...args)
    {
        ntdcs_ = ntdcs;

        for (int i = 0; i < ntdcs_; i++)
        {
            tdc_.push_back(
                new H(
                    Form("%s_tdc_%d", name_, i),
                    Form("TDC %d", i),
                    std::forward<ARGS>(args)...
                )
            );
        }
    }

    H& operator[](size_t i)
    {
        return *tdc_[i];
    }

    const H& operator[](size_t i) const
    {
        return *tdc_[i];
    }

    template <class ...ARGS, class R, class F>
    void for_each(R (F::* function)(ARGS...), ARGS ...args)
    {
        for (auto it = tdc_.begin(); it != tdc_.end(); it++)
        {
            ((*it)->*function)(std::forward<ARGS>(args)...);
        }
    }

    template <class F>
    void for_each(F&& lambda) {
        for (auto it = tdc_.begin(); it != tdc_.end(); it++)
        {
            lambda(*it);
        }
    }

    void configure(TString options="")
    {
        // Configure maximum
        // Find maximum
        double max = 0;
        for (auto it = tdc_.begin(); it != tdc_.end(); it++)
        {
            if ((*it)->GetMaximum() > max) max = (*it)->GetMaximum();
        }
        // Set maximum
        for (auto it = tdc_.begin(); it != tdc_.end(); it++)
        {
            (*it)->SetMaximum(max*1.1);
        }

        // Configure bar chart look
        for (auto it = tdc_.begin(); it != tdc_.end(); it++)
        {
            (*it)->SetFillColor(16); // Bar color
            (*it)->SetLineColor(kBlack); // Error bar color
            (*it)->SetBarWidth(0.8); // Set bar width
            (*it)->SetBarOffset(0.1);
        }

        TString token;
        Ssiz_t from = 0;
        while (options.Tokenize(token, from, "[ ]"))
        {
            // Configure separate TDCs
            if (token == "SEP")
            {
                separate_ = true;
                continue;
            }

            // Configure color
            if (token == "COLZ")
            {
                for (auto tdc = 0; tdc < ntdcs_; tdc++)
                {
                    if (tdc < 3)           tdc_[tdc]->SetOption("COLZ");
                    else if (tdc % 2 == 0) tdc_[tdc]->SetOption("COLZ");
                    else if (!separate_)    tdc_[tdc]->SetOption("COL");
                    else                   tdc_[tdc]->SetOption("COLZ");
                }
                continue;
            }
            
            // Configure axes for strip/channel
            if (token == "XNUMS")
            {
                for (auto it = tdc_.begin(); it != tdc_.end(); it++)
                {
                    for (int i = 0; i < 32; i++)
                    {
                        (*it)->GetXaxis()->SetBinLabel(i+1, Form("%d", i));
                    }
                }
                continue;
            }
            
            // Default: configure "token" with SetOption
            for (auto tdc = 0; tdc < ntdcs_; tdc++)
            {
                tdc_[tdc]->SetOption(token);
            }
            
        }

        if (!separate_)
        {
            // Hide tick marks on right-side phi
            for (int tdc = 0; tdc < ntdcs_; tdc++)
            {
                if (tdc < 3)           {}
                else if (tdc % 2 == 0) tdc_[tdc]->GetYaxis()->SetTickLength(0);
                else                   {}
            }
        }
    }

    void configure_titles(const char* xtitle, const char* ytitle)
    {
        for (auto it = tdc_.begin(); it != tdc_.end(); it++)
        {
            (*it)->GetXaxis()->SetTitle(xtitle);
            (*it)->GetYaxis()->SetTitle(ytitle);
        }
    }

    void draw(TCanvas* canvas, bool set=0, const char* options="", bool same=false)
    {
        if (ntdcs_ > 0 && set == 0)
        {
            // Create sub-pads
            if (!same) canvas->Divide(3, 3, 0, 0);

            for (int tdc = 0; tdc < ntdcs_; tdc++)
            {
                // Set sub-pad
                if (tdc < 3) canvas->cd(tdc*3+1);
                else if (tdc < 5) canvas->cd(tdc-1);
                else if (tdc < 7) canvas->cd(tdc);
                else if (tdc < 9) canvas->cd(tdc+1);

                if (!separate_)
                {
                    // Configure margins                    L    R    T    B
                    if (tdc < 3)           gPad->SetMargin(0.1, 0.1, 0.1, 0.1);
                    else if (tdc % 2 == 0) gPad->SetMargin(0.0, 0.1, 0.1, 0.1);
                    else                   gPad->SetMargin(0.1, 0.0, 0.1, 0.1);
                }
                else
                {
                    // Configure margins to be equal
                    gPad->SetMargin(0.1, 0.1, 0.1, 0.1);
                }
                

                // Draw histogram to sub-pad
                tdc_[tdc]->Draw(options);
            }
        }
        if (ntdcs_ >= 9 && set == 1)
        {
            // Create sub-pads
            canvas->Divide(3, 3);

            for (int tdc = 9; tdc < ntdcs_; tdc++)
            {
                // Set sub-pad
                // TODO: Fix this to use correct BIS8 mapping
                if (tdc-9 < 3) canvas->cd(tdc*3+1);
                else if (tdc-9 < 5) canvas->cd(tdc-1);
                else if (tdc-9 < 7) canvas->cd(tdc);
                else if (tdc-9 < 9) canvas->cd(tdc+1);

                // Draw histogram to sub-pad
                tdc_[tdc]->Draw(options);
            }
        }
    }
};


#endif // SUMMARY_HIST_H
