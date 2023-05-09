#ifndef SP_RECTS_H
#define SP_RECTS_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class sp_rects; }
QT_END_NAMESPACE

struct RectCoord
{
    int m_x1;
    int m_y1;
    int m_x2;
    int m_y2;

    RectCoord(int x1,int y1,int x2, int y2)
    {
        m_x1 = x1;
        m_y1 = y1;
        m_x2 = x2;
        m_y2 = y2;
    }

    bool operator==(const RectCoord& rhs)
    {
        return ((this->m_x1 == rhs.m_x1) && (this->m_y1 == rhs.m_y1) &&
                (this->m_x2 == rhs.m_x2) && (this->m_y2 == rhs.m_y2));
    }
};

class sp_rects : public QMainWindow
{
    Q_OBJECT

public:
    sp_rects(QWidget *parent = nullptr);
    ~sp_rects();

    bool addRect(int, int, int, int);
    void paintEvent(QPaintEvent *event);
    bool isVisible(unsigned int, unsigned int);
    int recDistance(unsigned int num1, unsigned int num2);
    void createGraph();
    void dijkstra(std::vector<std::vector<int>> &graph, int src, int dest);
    void addLine(unsigned int num1, unsigned int num2);



    private slots:

    void on_add_rect_button_clicked();

    void on_delete_rect_button_clicked();

    void on_show_graph_button_clicked();

    void on_info_button_clicked();

    void on_upload_file_button_clicked();

private:
    Ui::sp_rects *ui;
    QList<QRect> rects;
    QList<RectCoord> coords;
    QList<QLine> lines;
    QList<QString> coord_list;
    std::vector<std::vector<int>> graph;
    unsigned int recCount;

    int minDistance(int dist[], bool sptSet[]);
    bool isNumber(const QString&& str);
};
#endif // SP_RECTS_H
