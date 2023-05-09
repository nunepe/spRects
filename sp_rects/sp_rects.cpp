#include "sp_rects.h"
#include "ui_sp_rects.h"
#include <QtGui>
#include <QtCore>
#include <QRectF>
#include <QString>
#include <QMessageBox>
#include <QFileDialog>

sp_rects::sp_rects(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::sp_rects)
{
    setAttribute(Qt::WA_OpaquePaintEvent, false);
    ui->setupUi(this);
}

sp_rects::~sp_rects()
{
    delete ui;
}

void sp_rects::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    QPixmap pixmap(":/txt.png");
    int x = 825;
    int y = 140;
    int width = pixmap.width();
    int height = pixmap.height();
    QRect imageRect = QRect(x, y, width, height);
    painter.drawPixmap(imageRect, pixmap);


    QPen pen1(Qt::black);
    pen1.setWidth(3);
    painter.setBrush(Qt::green);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(pen1);
    painter.drawRects(rects);

    QPen pen2(Qt::black);
    QFont font("Arial", 16);
    painter.setFont(font);
    painter.setPen(pen2);
    for (unsigned int i = 0; i < rects.size(); ++i)
        painter.drawText(rects[i].left() + 5, rects[i].top() + 5, rects[i].width(), rects[i].height(),Qt::AlignTop | Qt::AlignLeft, QString::number(i));


    if (!lines.isEmpty())
    {
        QPen pen3(Qt::DashLine);
        pen3.setColor(Qt::red);
        pen3.setWidth(5);
        painter.setPen(pen3);
        painter.drawLines(lines);
    }
}

bool sp_rects::addRect(int x1, int y1, int x2, int y2)
{
    if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0 ||
    x1 > 800 || y1 > 800 || x2 > 800 || y2 > 800)
    {
        QMessageBox::warning(this, "Error", "Enter the coordinates between [0; 80]");
        return false;
    }
    if (x1 == x2 || y1 == y2)
    {
        QMessageBox::warning(this, "Error", "The difference between coordinates must be minimum 1");
        return false;
    }
    QRect rect(x1,y1,x2 - x1, y2 - y1);
    for (unsigned int i = 0; i < rects.size(); ++i)
    {
        if (rect == rects[i])
        {
            QMessageBox::warning(this, "Error", "Rectangle with given coordinates already exists\nEnter another one");
            return false;
        }
    }
    if (x1 < x2 && y1 < y2)
    {
        RectCoord coord(x1,y1,x2,y2);
        rects.push_back(rect);
        coords.push_back(coord);
        this->recCount = rects.size();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Let the coordinates x1 and y1 be less than x2 and y2");
        return false;
    }
    return true;
}

bool sp_rects::isVisible(unsigned int num1, unsigned int num2)
{
    for (unsigned int i = 0; i < unsigned(recCount); ++i)
    {
        if (i == num1 || i == num2)
            continue;
        if (rects[i].contains(rects[num1]) || rects[i].contains(rects[num2]))
        {
            return false;
        }
    }
    bool isVisible = false;
    if (rects[num1].intersects(rects[num2]))
        return true;
    if ((coords[num1].m_x1 >= coords[num2].m_x1 && coords[num1].m_x1 <= coords[num2].m_x2) ||
    (coords[num1].m_x2 >= coords[num2].m_x1 && coords[num1].m_x2 <= coords[num2].m_x2) ||
    (coords[num1].m_y1 >= coords[num2].m_y1 && coords[num1].m_y1 <= coords[num2].m_y2) ||
    (coords[num1].m_y2 >= coords[num2].m_y1 && coords[num1].m_y2 <= coords[num2].m_y2) ||

    (coords[num2].m_x1 >= coords[num1].m_x1 && coords[num2].m_x1 <= coords[num1].m_x2) ||
    (coords[num2].m_x2 >= coords[num1].m_x1 && coords[num2].m_x2 <= coords[num1].m_x2) ||
    (coords[num2].m_y1 >= coords[num1].m_y1 && coords[num2].m_y1 <= coords[num1].m_y2) ||
    (coords[num2].m_y2 >= coords[num1].m_y1 && coords[num2].m_y2 <= coords[num1].m_y2))
    {
        isVisible = true;
    }
    for (unsigned int i = 0; i < unsigned(recCount); ++i)
    {
        if (i == num1 || i == num2)
            continue;
        if (rects[i].contains(rects[num1]) || rects[i].contains(rects[num2]) ||
        (coords[i].m_x1 < coords[num2].m_x1 && coords[i].m_x2 > coords[num2].m_x2 &&
        coords[i].m_y1 > coords[num1].m_y2 && coords[i].m_y2 < coords[num2].m_y1) ||
        (coords[i].m_x1 < coords[num1].m_x1 && coords[i].m_x2 > coords[num1].m_x2 &&
        coords[i].m_y1 > coords[num2].m_y2 && coords[i].m_y2 < coords[num1].m_y1) ||

        (coords[i].m_y1 < coords[num2].m_y1 && coords[i].m_y2 > coords[num2].m_y2 &&
        coords[i].m_x1 > coords[num1].m_x2 && coords[i].m_x2 < coords[num2].m_x1) ||
        (coords[i].m_y1 < coords[num1].m_y1 && coords[i].m_y2 > coords[num1].m_y2 &&
        coords[i].m_x1 > coords[num2].m_x2 && coords[i].m_x2 < coords[num1].m_x1))
        {
            isVisible = false;
            break;
        }
    }
    return isVisible;
}

bool sp_rects::isNumber(const QString &&str)
{
    if (str.isEmpty())
    {
        return false;
    }
    for (int i = 0; i < str.size(); ++i)
    {
        if (!str[i].isDigit())
        {
            return false;
        }
    }
    return true;
}

void sp_rects::on_add_rect_button_clicked()
{
    if (isNumber(ui->x1->text()) && isNumber(ui->y1->text()) &&
        isNumber(ui->x2->text()) && isNumber(ui->y2->text()))
    {
        int x1 = ui->x1->text().toInt() * 10;
        int y1 = ui->y1->text().toInt() * 10;
        int x2 = ui->x2->text().toInt() * 10;
        int y2 = ui->y2->text().toInt() * 10;
        if (addRect(x1,y1,x2,y2))
        {
            coord_list.push_back(/*"Rect " + QString::number(rects.size() - 1) + */" {(" +
                                        QString::number(x1 / 10) + "," + QString::number(y1 / 10) + "); (" +
                                        QString::number(x2 / 10) + "," + QString::number(y2 / 10) + ")}");
            ui->show_coordinates->clear();
            for (int i = 0; i < coord_list.size(); ++i)
            {
                ui->show_coordinates->append("Rect " + QString::number(i));
                ui->show_coordinates->moveCursor(QTextCursor::End);
                ui->show_coordinates->insertPlainText(coord_list[i]);
            }
            ui->show_coordinates->verticalScrollBar();
        }
        else
            return;
    }
    else
    {
        if (ui->x1->text().isEmpty() || ui->y1->text().isEmpty() ||
            ui->x2->text().isEmpty() || ui->y2->text().isEmpty())
        {
            QMessageBox::warning(this, "Error", "Empty fields not allowed");
        }
        else
        {
            QMessageBox::warning(this, "Error", "Only numbers allowed");
        }
    }
    ui->x1->clear();
    ui->y1->clear();
    ui->x2->clear();
    ui->y2->clear();
    ui->show_graph->clear();
    this->update();
    return;
}

int sp_rects::recDistance(unsigned int num1, unsigned int num2)
{
    if (rects[num1].intersects(rects[num2]))
    {
        return 0;
    }
    else if (coords[num1].m_x2 < coords[num2].m_x1)
    {
        return coords[num2].m_x1 - coords[num1].m_x2;
    }
    else if (coords[num1].m_x1 > coords[num2].m_x2)
    {
        return coords[num1].m_x1 - coords[num2].m_x2;
    }
    else if (coords[num1].m_y2 <= coords[num2].m_y1)
    {
        return coords[num2].m_y1 - coords[num1].m_y2;
    }
    else if (coords[num1].m_y1 >= coords[num2].m_y2)
    {
        return coords[num1].m_y1 - coords[num2].m_y2;
    }
    return 0;
}

void sp_rects::createGraph()
{
    this->graph.resize(rects.size());
    for (unsigned int i = 0; i < graph.size(); ++i)
    {
        graph[i].resize(rects.size());
    }
    for (unsigned int i = 0; i < graph.size(); ++i)
    {
        for (unsigned int j = 0; j < graph.size(); ++j)
        {
            if (isVisible(i, j))
            {
                this->graph[i][j] = recDistance(i, j);
            }
            else
            {
                this->graph[i][j] = INT_MAX;
            }
        }
    }
}

void sp_rects::on_show_graph_button_clicked()
{
    if (isNumber(ui->source_rec->text()) && isNumber(ui->dest_rec->text()))
    {
        ui->show_graph->clear();
        int src = ui->source_rec->text().toInt();
        int dest = ui->dest_rec->text().toInt();
        if (src >= 0 && src < rects.size() &&
            dest >= 0 && dest < rects.size())
        {
            createGraph();
            dijkstra(graph, src, dest);
        }
        else
        {
            QMessageBox::warning(this, "Error", "Rectangle with such index doesn't exist");
        }

        this->update();
    }
    else
    {
        if (ui->source_rec->text().isEmpty() || ui->dest_rec->text().isEmpty())
        {
            QMessageBox::warning(this, "Error", "Empty fields not allowed");
        }
        else
        {
            QMessageBox::warning(this, "Error", "Only numbers allowed");
        }
    }
    ui->source_rec->clear();
    ui->dest_rec->clear();
}

void sp_rects::addLine(unsigned int num1, unsigned int num2)
{
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

    if (coords[num1].m_x2 < coords[num2].m_x1)
    {
        x1 = coords[num1].m_x2;
        x2 = coords[num2].m_x1;
        if ((coords[num1].m_y2 <= coords[num2].m_y2) && (coords[num1].m_y1 <= coords[num2].m_y1))
        {
            y1 = (coords[num1].m_y2 + coords[num2].m_y1) / 2;
            y2 = y1;
        }
        else if ((coords[num1].m_y2 <= coords[num2].m_y2) && (coords[num1].m_y1 >= coords[num2].m_y1))
        {
            y1 = (coords[num1].m_y1 + coords[num1].m_y2) / 2;
            y2 = y1;
        }
        else if ((coords[num1].m_y2 >= coords[num2].m_y2) && (coords[num1].m_y1 <= coords[num2].m_y1))
        {
            y1 = (coords[num2].m_y1 + coords[num2].m_y2) / 2;
            y2 = y1;
        }
        else if ((coords[num1].m_y2 >= coords[num2].m_y2) && (coords[num1].m_y1 >= coords[num2].m_y1))
        {
            y1 = (coords[num1].m_y1 + coords[num2].m_y2) / 2;
            y2 = y1;
        }
    }

    else if (coords[num2].m_x2 < coords[num1].m_x1)
    {
        x1 = coords[num2].m_x2;
        x2 = coords[num1].m_x1;
        if ((coords[num2].m_y2 <= coords[num1].m_y2) && (coords[num2].m_y1 <= coords[num1].m_y1))
        {
            y1 = (coords[num2].m_y2 + coords[num1].m_y1) / 2;
            y2 = y1;
        }
        else if ((coords[num2].m_y2 <= coords[num1].m_y2) && (coords[num2].m_y1 >= coords[num1].m_y1))
        {
            y1 = (coords[num2].m_y1 + coords[num2].m_y2) / 2;
            y2 = y1;
        }
        else if ((coords[num2].m_y2 >= coords[num1].m_y2) && (coords[num2].m_y1 <= coords[num1].m_y1))
        {
            y1 = (coords[num1].m_y1 + coords[num1].m_y2) / 2;
            y2 = y1;
        }
        else if ((coords[num2].m_y2 >= coords[num1].m_y2) && (coords[num2].m_y1 >= coords[num1].m_y1))
        {
            y1 = (coords[num2].m_y1 + coords[num1].m_y2) / 2;
            y2 = y1;
        }
    }

    else if (coords[num1].m_y2 < coords[num2].m_y1)
    {
        y1 = coords[num1].m_y2;
        y2 = coords[num2].m_y1;
        if ((coords[num1].m_x1 <= coords[num2].m_x1) && (coords[num1].m_x2 <= coords[num2].m_x2))
        {
            x1 = (coords[num1].m_x2 + coords[num2].m_x1) / 2;
            x2 = x1;
        }
        else if ((coords[num1].m_x1 >= coords[num2].m_x1) && (coords[num1].m_x2 <= coords[num2].m_x2))
        {
            x1 = (coords[num1].m_x1 + coords[num1].m_x2) / 2;
            x2 = x1;
        }
        else if ((coords[num1].m_x1 <= coords[num2].m_x1) && (coords[num1].m_x2 >= coords[num2].m_x2))
        {
            x1 = (coords[num2].m_x1 + coords[num2].m_x2) / 2;
            x2 = x1;
        }
        else if ((coords[num1].m_x1 >= coords[num2].m_x1) && (coords[num1].m_x2 >= coords[num2].m_x2))
        {
            x1 = (coords[num2].m_x2 + coords[num1].m_x1) / 2;
            x2 = x1;
        }
    }

    else if (coords[num2].m_y2 < coords[num1].m_y1)
    {
        y1 = coords[num2].m_y2;
        y2 = coords[num1].m_y1;
        if ((coords[num2].m_x1 <= coords[num1].m_x1) && (coords[num2].m_x2 <= coords[num1].m_x2))
        {
            x1 = (coords[num2].m_x2 + coords[num1].m_x1) / 2;
            x2 = x1;
        }
        else if ((coords[num2].m_x1 >= coords[num1].m_x1) && (coords[num2].m_x2 <= coords[num1].m_x2))
        {
            x1 = (coords[num2].m_x1 + coords[num2].m_x2) / 2;
            x2 = x1;
        }
        else if ((coords[num2].m_x1 <= coords[num1].m_x1) && (coords[num2].m_x2 >= coords[num1].m_x2))
        {
            x1 = (coords[num1].m_x1 + coords[num1].m_x2) / 2;
            x2 = x1;
        }
        else if ((coords[num2].m_x1 >= coords[num1].m_x1) && (coords[num2].m_x2 >= coords[num1].m_x2))
        {
            x1 = (coords[num1].m_x2 + coords[num2].m_x1) / 2;
            x2 = x1;
        }
    }

    else
        return;

    QLine line(x1 ,y1, x2, y2);
    lines.append(line);
}

int sp_rects::minDistance(int dist[], bool sptSet[])
{
    int min = INT_MAX, min_index = -1;

    for (unsigned int v = 0; v < recCount; v++)
    {
        if (sptSet[v] == false && dist[v] <= min)
        {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void sp_rects::dijkstra(std::vector<std::vector<int>> &graph, int src, int dest)
{
    int dist[recCount];
    int parent[recCount];
    bool sptSet[recCount];

    for (unsigned int i = 0; i < recCount; i++)
    {
        dist[i] = INT_MAX;
        parent[i] = -1;
        sptSet[i] = false;
    }

    dist[src] = 0;
    for (unsigned int count = 0; count < recCount - 1; count++)
    {
        int u = minDistance(dist, sptSet);
        sptSet[u] = true;
        for (unsigned int v = 0; v < recCount; v++)
        {
            if (!sptSet[v] && graph[u][v] != INT_MAX && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
            {
                dist[v] = dist[u] + graph[u][v];
                parent[v] = u;
            }
        }
    }
    std::vector<int> path;
    path.push_back(dest);

    int cur = dest;
    while (parent[cur] != -1)
    {
        path.push_back(parent[cur]);
        cur = parent[cur];
    }
    lines.clear();

    for (int i = path.size() - 1; i > 0; --i)
    {
        addLine(path[i], path[i - 1]);
    }

    if (dist[dest] == INT_MAX)
    {
            ui->show_graph->append("The destination " + QString::number(dest) + " is unreachable from the source " + QString::number(src));
    }
    else
    {
        ui->show_graph->append(" The shortest distance from " + QString::number(src) + " to " + QString::number(dest) + " is [" + QString::number(dist[dest] / 10) + "]\n\n");
        ui->show_graph->moveCursor(QTextCursor::End);
        ui->show_graph->insertPlainText("The shortest path is:\n[");
        for (int i = path.size() - 1; i >= 0; i--)
        {
            ui->show_graph->moveCursor(QTextCursor::End);
            ui->show_graph->insertPlainText(QString::number(path[i]));
            if (i > 0)
            {
                ui->show_graph->moveCursor(QTextCursor::End);
                ui->show_graph->insertPlainText(" -> ");
            }
            else
            {
                ui->show_graph->moveCursor(QTextCursor::End);
                ui->show_graph->insertPlainText("]");
            }
            ui->show_graph->verticalScrollBar();
        }
    }
}


void sp_rects::on_info_button_clicked()
{
    QMessageBox::information(this, tr("Info"), tr("Application Usage:\n"
                                                  "1.Enter top-left(x1,y1) and bottom-right(x2,y2) coordinates of a rectangle\n"
                                                  "    •You must enter only numbers\n"
                                                  "    •The coordinates x1 and y1 must be less than x2 and y2\n"
                                                  "    •The coordinates x1 and y1 can differ from x2 and y2 by at least 1\n"
                                                  "    •You can't enter coordinates which are already used\n"
                                                  "2.Enter any source and destination to calculate shortest path between them\n"
                                                  "    •You must enter only numbers\n"));

}

void sp_rects::on_delete_rect_button_clicked()
{
    if (isNumber(ui->deleting_rect->text()))
    {
        int index = ui->deleting_rect->text().toInt();
        if (index >= 0 && index < rects.size())
        {
            auto it = std::find(rects.begin(), rects.end(), rects[index]);
            std::rotate(it, it + 1, rects.end());
            rects.pop_back();

            auto it1 = std::find(coords.begin(), coords.end(), coords[index]);
            std::rotate(it1, it1 + 1, coords.end());
            coords.pop_back();

            auto it2 = std::find(coord_list.begin(), coord_list.end(), coord_list[index]);
            std::rotate(it2, it2 + 1, coord_list.end());
            coord_list.pop_back();

            ui->show_coordinates->clear();
            for (int i = 0; i < coord_list.size(); ++i)
            {
                ui->show_coordinates->append("Rect " + QString::number(i));
                ui->show_coordinates->moveCursor(QTextCursor::End);
                ui->show_coordinates->insertPlainText(coord_list[i]);
            }
            ui->show_coordinates->verticalScrollBar();
            ui->show_graph->clear();
            ui->source_rec->clear();
            ui->dest_rec->clear();
            recCount--;
            createGraph();
            lines.clear();
        }
        else
            QMessageBox::warning(this, "Error", "Rectangle with such index doesn't exist");

        this->update();
    }
    else
    {
        if (ui->deleting_rect->text().isEmpty())
        {
            QMessageBox::warning(this, "Error", "Empty fields not allowed");
        }
        else
        {
            QMessageBox::warning(this, "Error", "Only numbers allowed");
        }
    }
    ui->deleting_rect->clear();

}


void sp_rects::on_upload_file_button_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/Users/arshogafaryan/sp_rects", tr("Text Files (*.txt)"));

    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            while (!in.atEnd())
            {
                QString line = in.readLine();
                QRegularExpression regex("\\((-?\\d+),(-?\\d+)\\); \\((-?\\d+),(-?\\d+)\\)");
                QRegularExpressionMatch match = regex.match(line);

                if (match.hasMatch())
                {
                    int x1 = match.captured(1).toInt() * 10;
                    int y1 = match.captured(2).toInt() * 10;
                    int x2 = match.captured(3).toInt() * 10;
                    int y2 = match.captured(4).toInt() * 10;
                    if (addRect(x1,y1,x2,y2))
                    {
                        coord_list.push_back(/*"Rect " + QString::number(rects.size() - 1) + */" {(" +
                                                    QString::number(x1 / 10) + "," + QString::number(y1 / 10) + "); (" +
                                                    QString::number(x2 / 10) + "," + QString::number(y2 / 10) + ")}");
                        ui->show_coordinates->clear();
                        for (int i = 0; i < coord_list.size(); ++i)
                        {
                            ui->show_coordinates->append("Rect " + QString::number(i));
                            ui->show_coordinates->moveCursor(QTextCursor::End);
                            ui->show_coordinates->insertPlainText(coord_list[i]);
                        }
                        ui->show_coordinates->verticalScrollBar();
                    }
                    else
                    {
                        return;
                    }
                }
                else
                {
                    QMessageBox::warning(this, "Error", "Input format in file is not allowed");
                    break;
                }

            }
            file.close();
        } else {
            QMessageBox::warning(this, "Error", "Could not open the file");
        }
    } else {
        QMessageBox::warning(this, "Error", "No file selected");
    }
}

