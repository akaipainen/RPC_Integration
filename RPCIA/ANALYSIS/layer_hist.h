#if !defined(LAYER_HIST_H)
#define LAYER_HIST_H

#include <vector>
#include <iostream>

#include <TString.h>
#include <TH1.h>
#include <TCanvas.h>

template <class H>
class LayerHist
{
private:
    std::vector<H*> layer_;

    const char* name_;
    
public:
    // Constructor
    LayerHist(const char* name)
     : name_(name)
    {
    }

    // Destructor
    ~LayerHist()
    {
        // Delete all histograms
        for (auto it = layer_.begin(); it != layer_.end(); it++)
        {
            delete *it;
        }
    }

    // Copy constructor
    LayerHist(const LayerHist<H>& other)
    {
        for (auto it = other.layer_.begin(); it != other.layer_.end(); it++)
        {
            layer_.push_back(new H(**it));
        }
    }

    // Copy assignment operator
    LayerHist<H>& operator=(const LayerHist<H>& other)
    {
        auto temp(other);
        std::swap(layer_, temp.layer_);

        return *this;
    }

    // Move constructor
    LayerHist(LayerHist<H> &&other)
    {
        layer_ = other.layer_;

        other.layer_.clear();
    }

    // Move assignment operator
    LayerHist<H>& operator=(LayerHist<H> &&other)
    {
        layer_ = std::move(other.layer_);
        return *this;
    }

    template <class ...ARGS>
    void init(int ntdcs, ARGS && ...args)
    {
        for (int i = 0; i < 6; i++)
        {
            if (i < 3)
            {
                layer_.push_back(
                    new H(
                        Form("%s_layer_%d", name_, i),
                        Form("Eta Layer %d", i%3),
                        std::forward<ARGS>(args)...
                    )
                );
            }
            else
            {
                layer_.push_back(
                    new H(
                        Form("%s_layer_%d", name_, i),
                        Form("Phi Layer %d", i%3),
                        std::forward<ARGS>(args)...
                    )
                );
            }
            
        }
    }

    H& operator[](size_t i)
    {
        return *layer_[i];
    }

    const H& operator[](size_t i) const
    {
        return *layer_[i];
    }

    template <class ...ARGS, class R, class F>
    void for_each(R (F::* function)(ARGS...), ARGS ...args)
    {
        for (auto it = layer_.begin(); it != layer_.end(); it++)
        {
            ((*it)->*function)(std::forward<ARGS>(args)...);
        }
    }

    template <class F>
    void for_each(F&& lambda) {
        for (auto it = layer_.begin(); it != layer_.end(); it++)
        {
            lambda(*it);
        }
    }

    void configure(TString options="")
    {
        // Configure maximum
        // Find maximum
        double max = 0;
        for (auto it = layer_.begin(); it != layer_.end(); it++)
        {
            if ((*it)->GetMaximum() > max) max = (*it)->GetMaximum();
        }
        // Set maximum
        for (auto it = layer_.begin(); it != layer_.end(); it++)
        {
            (*it)->SetMaximum(max*1.1);
        }

        TString token;
        Ssiz_t from = 0;
        while (options.Tokenize(token, from, "[ ]"))
        {
            
            // Configure axes for strip/channel
            if (token == "XNUMS")
            {
                for (auto it = layer_.begin(); it != layer_.end(); it++)
                {
                    for (int i = 0; i < 32; i++)
                    {
                        (*it)->GetXaxis()->SetBinLabel(i+1, Form("%d", i));
                    }
                }
                continue;
            }
            
            // Default: configure "token" with SetOption
            for (auto &layer: layer_)
            {
                layer->SetOption(token);
            }
            
        }
    }

    void configure_titles(const char* xtitle, const char* ytitle)
    {
        for (auto it = layer_.begin(); it != layer_.end(); it++)
        {
            (*it)->GetXaxis()->SetTitle(xtitle);
            (*it)->GetYaxis()->SetTitle(ytitle);
        }
    }

    void draw(TCanvas* canvas, bool set=0, const char* options="", bool log=false)
    {
        // Create sub-pads
        canvas->Divide(2, 3, 0, 0);

        gStyle->SetOptStat(1111);

        for (int layer = 0; layer < 6; layer++)
        {
            // Set sub-pad
            if      (layer < 3) canvas->cd(layer*2 + 1);
            else if (layer == 3) canvas->cd(2);
            else if (layer == 4) canvas->cd(4);
            else if (layer == 5) canvas->cd(6);

            if (log) gPad->SetLogy();

            // Configure margins
            gPad->SetMargin(0.1, 0.1, 0.1, 0.1);

            layer_[layer]->Draw(options);
        }
    }
};


#endif // LAYER_HIST_H
