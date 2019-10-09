#pragma once

#include <LibGUI/GWindow.h>

class GTableView;
class GTextBox;

class VBPropertiesWindow final : public GWindow {
    C_OBJECT(VBPropertiesWindow)
public:
    VBPropertiesWindow();
    virtual ~VBPropertiesWindow() override;

    GTableView& table_view() {
        return *m_table_view;
    }
    const GTableView& table_view() const {
        return *m_table_view;
    }

private:
    RefPtr<GTableView> m_table_view;
};
