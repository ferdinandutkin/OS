#include "framework.h"


namespace  formatting {
	void set_font(const HWND handle_window, HFONT& init_font) {
		LOGFONTW log_font;

		GetObjectW(init_font, sizeof(LOGFONTW), &log_font);


		CHOOSEFONT choose_font{ sizeof(CHOOSEFONT) };

		choose_font.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;

		choose_font.lpLogFont = &log_font;


		if (ChooseFontW(&choose_font)) {
			HFONT result_font = CreateFontIndirectW(&log_font);
			init_font = result_font;
			SendMessageW(handle_window, WM_SETFONT, reinterpret_cast<WPARAM>(init_font), true);

		}
	}


	void set_color(const HWND handle_window, COLORREF& init_color) {

		static COLORREF cust_colors[16]{};


		CHOOSECOLOR choose_color{ sizeof(CHOOSECOLOR) };


		choose_color.rgbResult = init_color;
		choose_color.lpCustColors = cust_colors;
		choose_color.Flags = CC_RGBINIT | CC_FULLOPEN | CC_ANYCOLOR;
		choose_color.hwndOwner = GetParent(handle_window);


		if (ChooseColorW(&choose_color)) {
			init_color = choose_color.rgbResult;
			RedrawWindow(handle_window, NULL, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);

		};
	}



	typedef void (*font_function)(HWND, HFONT&);

	template <bool italic, bool strike_out, bool underline>
	void toggle_style(const HWND handle_window, HFONT& init_font) {
		LOGFONTW log_font;
		GetObjectW(init_font, sizeof(LOGFONTW), &log_font);

		log_font.lfItalic ^= italic;
		log_font.lfStrikeOut ^= strike_out;
		log_font.lfUnderline ^= underline;

		HFONT result_font = CreateFontIndirectW(&log_font);
		init_font = result_font;
		SendMessageW(handle_window, WM_SETFONT, reinterpret_cast<WPARAM>(init_font), true);
	}

	void toggle_bold(const HWND handle_window, HFONT& init_font) {
		LOGFONTW log_font;
		GetObjectW(init_font, sizeof(LOGFONTW), &log_font);

		static LONG prev_weight;

		if (log_font.lfWeight != FW_BOLD) {
			prev_weight = log_font.lfWeight;
			log_font.lfWeight = FW_BOLD;
		}
		else {
			log_font.lfWeight = prev_weight;
		}

		HFONT result_font = CreateFontIndirectW(&log_font);
		init_font = result_font;
		SendMessageW(handle_window, WM_SETFONT, reinterpret_cast<WPARAM>(init_font), true);
	}

	font_function toggle_italic = toggle_style<true, false, false>;
	font_function toggle_strike_out = toggle_style<false, true, false>;
	font_function toggle_underline = toggle_style<false, false, true>;

}



