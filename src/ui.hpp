/*
    This file is part of Roboverb

    Copyright (C) 2025  Kushview, LLC.  All rights reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <functional>

#include <clap/clap.h>

#include <lui/button.hpp>
#include <lui/cairo.hpp>
#include <lui/slider.hpp>
#include <lui/widget.hpp>

#include "ports.hpp"
#include "res.hpp"

namespace roboverb {

/** Color table for the comb and all pass toggles.
    ordered by rows and columns going top-down left-to-right.
*/
static const lui::Color _toggle_colors[] = {
    0xffffa500,
    0xffefa500,
    0xffdfa500,
    0xffcfa500,
    0xffffa500,
    0xffefa500,
    0xffdfa500,
    0xffcfa500,
    0xffffa500,
    0xffefa500,
    0xffdfa500,
    0xffcfa500
};

class Toggle : public lui::Button {
public:
    Toggle() {
        bgImg = lui::Image::load ((const uint8_t*) res::toggle_switch_png, res::toggle_switch_pngSize);
        set_size (preferedSize(), preferedSize());
    }

    ~Toggle() = default;

    void setOnColor (lui::Color color) {
        _color_on = color;
        repaint();
    }

    void setText (const std::string& text) {
        _text = text;
        repaint();
    }

    int preferedSize() const noexcept {
        return bgImg.width();
    }

protected:
    double imgScale = 0.5;
    void paint_button (lui::Graphics& g, bool highlight, bool down) override {
#if 1
        using Fit = lui::Fitment;
        lui::Transform mat;
        if (toggled())
            g.draw_image (bgImg, mat.translated (0.0, -(bgImg.height() / 2)));
        else
            g.draw_image (bgImg, mat);
#else
        g.set_color (0xff111111);
        auto b = bounds().at (0);
        g.fill_rect (b);

        g.set_color (toggled() ? _color_on : _color_off.with_alpha (0.8f));
        g.fill_rect (b.smaller (1));

        g.set_color (text_color());
        g.draw_text (_text, bounds().at (0).as<float>(), lui::Justify::CENTERED);
#endif
    }

private:
    lui::Image bgImg;
    float _text_alpha { 0.72f };
    lui::Color _color_on { 0xffffa400 },
        _color_off { 0xff363333 },
        _color_text_off { 0xffffffff },
        _color_text_on { 0xff222222 };
    std::string _text;

    lui::Color text_color() const noexcept {
        auto c = toggled() ? _color_text_on : _color_text_off;
        return c.with_alpha (_text_alpha);
    }
};

class ControlLabel : public lui::Widget {
public:
    ControlLabel (const std::string& text) {
        set_name (text);
        _text = name();
    }

    void paint (lui::Graphics& g) override {
        g.set_color (0xffffffff);
        g.set_font (lui::Font (11.f));
        g.draw_text (_text, bounds().at (0).as<float>(), _align);
    }

    void set_text (const std::string& text) {
        _text = text;
        repaint();
    }

private:
    lui::Justify _align { lui::Justify::MID_LEFT };
    std::string _text;
};

class Content : public lui::Widget {
public:
    std::function<void (uint32_t, float)> on_control_changed;

    Content() {
        set_opaque (true);
        bg_image = lui::Image::load ((uint8_t*) res::roboverb_bg_jpg, res::roboverb_bg_jpgSize);

        for (int i = Ports::Wet; i <= Ports::Width; ++i) {
            auto s = add (new lui::Slider());
            s->set_range (0.0, 1.0);
            s->set_type (lui::Slider::HORIZONTAL);

            s->on_value_changed = [&, i, s]() {
                if (on_control_changed) {
                    const auto port  = static_cast<uint32_t> (i);
                    const auto value = static_cast<float> (s->value());
                    on_control_changed (port, value);
                }
            };

            sliders.push_back (s);

            std::string text = "";
            switch (i) {
                case Ports::Wet:
                    text = "Wet level";
                    break;
                case Ports::Dry:
                    text = "Dry level";
                    break;
                case Ports::RoomSize:
                    text = "Room size";
                    break;
                case Ports::Damping:
                    text = "Damping";
                    break;
                case Ports::Width:
                    text = "Width";
                    break;
            }

            labels.push_back (add (new ControlLabel (text)));
        }

        for (int i = Ports::Comb_1; i <= Ports::AllPass_4; ++i) {
            auto idx = i - Ports::Comb_1;
            auto t   = add (new Toggle());
            t->toggle (false);
            t->setOnColor (_toggle_colors[idx]);
            std::stringstream text;
            if (i < Ports::AllPass_1)
                text << "C" << (idx + 1);
            else
                text << "A" << (i - Ports::AllPass_1 + 1);
            t->setText (text.str());
            t->on_clicked = [&, i, t]() {
                t->toggle (! t->toggled());
                if (on_control_changed) {
                    const auto port = static_cast<uint32_t> (i);
                    on_control_changed (port, t->toggled() ? 1.f : 0.f);
                }
            };

            toggles.push_back (t);
        }

        update_toggles();

        show_all();

        if (bg_image)
            set_size (bg_image.width(), bg_image.height());
        else
            set_size (640 * 0.72, 360 * 0.72);
    }

    ~Content() {
        for (auto t : toggles)
            delete t;
        toggles.clear();
        for (auto s : sliders)
            delete s;
        sliders.clear();
    }

    void update_toggles() {
        if (toggles.size() < 12)
            return;

        for (int i = Ports::Comb_1; i <= Ports::AllPass_4; ++i) {
            auto idx = i - Ports::Comb_1;
            auto t   = toggles.at (idx);
            t->setOnColor (_toggle_colors[idx]);

            if (_show_toggle_text) {
                std::stringstream text;
                if (i < Ports::AllPass_1)
                    text << "C" << (idx + 1);
                else
                    text << "A" << (i - Ports::AllPass_1 + 1);
                t->setText (text.str());
            }
        }
    }

    void update_toggle (int index, bool value) {
        if (! (index >= 0 && index < (int) toggles.size()))
            return;
        toggles[index]->toggle (value);
    }

    template <typename Ft>
    void update_slider (int index, Ft value) {
        if (! (index >= 0 && index < (int) sliders.size()))
            return;
        auto dvalue = static_cast<double> (value);
        sliders[index]->set_value (dvalue, lui::Notify::NONE);
        // std::clog << "[roboverb] slider_min ("<< sliders[index]->range().min <<")\n";
        // std::clog << "[roboverb] slider_max ("<< sliders[index]->range().max <<")\n";
        // std::clog << "[roboverb] slider_value ("<< sliders[index]->value() <<")\n";
    }

protected:
    void resized() override {
        const auto btn_size      = height() / 3 - 10;
        const auto btn_hspace    = btn_size * 4;
        const auto slider_hspace = width() - btn_hspace;

        auto r1 = bounds().at (0);
        auto sb = r1.slice_left (slider_hspace);
        sb.slice_top (40);
        sb.slice_bottom (20);
        int h = sb.height / 5;
        for (int i = 0; i < 5; ++i) {
            auto r = sb.slice_top (h);

            auto sr = r.slice_top (r.height * 0.3333);
            sr.slice_left (6);
            labels[i]->set_bounds (sr);
            r.slice_top (1);
            sliders[i]->set_bounds (r.smaller (3, 2));
        }

        auto tb           = r1.smaller (2); //bounds().at (0).slice_right (btn_hspace).smaller (2);
        size_t tidx       = 0;
        int actual_size   = tb.height / 3.f;
        int actual_h_size = tb.width / 4.f;
        for (int i = 0; i < 3; ++i) {
            auto r = tb.slice_top (actual_size);
            for (int j = 0; j < 4; ++j) {
                auto* t = toggles[tidx++];
                t->set_bounds (r.slice_left (actual_h_size)
                                   .smaller (1));
            }
        }
    }

    void paint (lui::Graphics& g) override {
        if (bg_image) {
            g.draw_image (bg_image, bounds().at (0).as<double>(), lui::Fitment::CENTERED);
        } else {
            g.set_color (lui::Color (255, 0, 0, 255));
            g.fill_rect (bounds().at (0));
        }
        g.set_color (0xff000000);
        g.set_font (16.0);
        g.draw_text (" ROBOVERB",
                     bounds().at (0).smaller (3, 4).as<float>(),
                     lui::Justify::TOP_LEFT);
    }

private:
    std::vector<lui::Slider*> sliders;
    std::vector<Toggle*> toggles;
    std::vector<ControlLabel*> labels;
    bool _show_toggle_text { true };
    lui::Image bg_image;
};

class GuiMain final {
    std::unique_ptr<lui::Main> gui;
    std::unique_ptr<Content> content;
    bool _elevated = false;

public:
    using ControlHandler = std::function<void (uint32_t port, float value)>;
    Content* widget() { return content.get(); }

    bool create() {
        if (gui == nullptr)
            gui = std::make_unique<lui::Main> (lui::Mode::MODULE, std::make_unique<lui::Cairo>());
        if (content == nullptr) {
            content = std::make_unique<Content>();
        }

        return gui != nullptr && content != nullptr;
    }

    void setControlHandler (ControlHandler handler) {
        if (content != nullptr)
            content->on_control_changed = handler;
    }

    bool destroy() {
        content = nullptr;
        gui     = nullptr;
        return true;
    }

    int width() const { return content->width(); }
    int height() const { return content->height(); }
    void show() { content->set_visible (true); }
    void hide() { content->set_visible (false); }

    bool setParent (const clap_window_t* parent) {
        if (_elevated)
            return true;
        if (content == nullptr || gui == nullptr)
            return false;
        return nullptr != gui->elevate (*content, 0, (uintptr_t) parent->ptr);
    }

    void idle() {
        if (gui)
            gui->loop (0);
    }

    uintptr_t nativeHandle() const noexcept {
        return content != nullptr ? content->find_handle() : 0;
    }
};

} // namespace roboverb
