//
// Created by mvideo on 01/03/26.
//

#include "visual_panel.hpp"
#include "visual_panel.hpp"

visual_panel::visual_panel(QWidget *parent) : QWidget(parent) {
    setup_ui();
}

void visual_panel::setup_ui() {
    auto* layout = new QVBoxLayout(this);

    m_headerLabel = new QLabel("Visual Chain Analysis", this);
    m_headerLabel->setObjectName("headerLabel");

    m_display = new QTextEdit(this);
    m_display->setReadOnly(true);
    m_display->setLineWrapMode(QTextEdit::WidgetWidth);
    layout->addWidget(m_headerLabel);
    layout->addWidget(m_display);
}

void visual_panel::display_iteration(const iteration_data& data) {
    m_display->clear();

    if (data.clusters.empty()) return;

    QString html = "<b>Generated Chain:</b><br><br>";

    for (size_t i = 0; i < data.clusters.size(); ++i) {
        html += QString::fromStdString(data.clusters[i].get_content());

        if (i < data.connections.size()) {
            const auto& conn = data.connections[i];
            if (conn.connected) {
                html += QString(" <b style='color:#FF85A2;'> <---> </b> ");
            } else {
                html += " <b style='color:#D0D0D0;'> | </b> ";
            }
        }
    }
    html += "<br><br><hr>";
    html += QString("Total connections: <b>%1</b>").arg(data.connected_count);

    m_display->setHtml(html);
}

void visual_panel::clear() {
    m_display->clear();
}