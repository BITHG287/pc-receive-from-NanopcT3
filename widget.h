#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUdpSocket>
#include <QHostAddress>
#include <QAudio>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QIODevice>
#include <QByteArray>
#include <QBuffer>
#include <QImage>
#include <QImageReader>
#include <QPixmap>
#include <QDebug>
//#include <QVector>
//using namespace std;

struct audio_t{
    char data[1024];
    int length;
};

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;

    QUdpSocket udp_audio_receiver;
    QUdpSocket udp_video_receiver;

    QAudioOutput *audio_out;
    QIODevice *audio_out_dev;

    audio_t au;

private slots:
    void udp_audio_recv_slot();
    void udp_video_recv_slot();

public:
    void init();

};

#endif // WIDGET_H
