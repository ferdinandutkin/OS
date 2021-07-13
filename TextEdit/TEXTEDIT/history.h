
#include "framework.h"

namespace history {
	struct history_menu {
	private:
		HMENU parent_menu;
		HMENU this_menu;
		UINT position;

		std::wstring name = L"История";

		UINT basic_id;

		bool shown = false;

		int count = 0;

	public:

		history_menu(const HWND handle_window, const UINT start_id, const UINT relative_pos) : position{ relative_pos }, basic_id{ start_id } {
			parent_menu = GetMenu(handle_window);
			this_menu = CreatePopupMenu();
		}

 
		void show() {
			if (!shown) {
				AppendMenuW(parent_menu, MF_POPUP, reinterpret_cast<UINT_PTR>(this_menu), name.c_str());
				shown = true;
			}

		}
		void hide() {
			if (shown) {
				RemoveMenu(parent_menu, position, MF_BYPOSITION);
				shown = false;
			}
		}

		void append(const std::wstring file) {
			AppendMenuW(this_menu, MF_ENABLED | MF_STRING, basic_id + count, file.c_str());
			count++;
		}

		~history_menu() {
			DestroyMenu(this_menu);
		}

		bool in_range(const UINT id) const {
			return (id >= basic_id) && (id <= (basic_id + count));
		}

		std::wstring get(const UINT id) const {
			std::wstring result;
			result.resize(MAX_PATH);
			result.resize(GetMenuStringW(this_menu, id - basic_id, result.data(), MAX_PATH, MF_BYPOSITION)) ;
		
			return result;
		}

	};


}
