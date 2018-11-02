#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    init();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
    udp_audio_receiver.bind(QHostAddress::Any, 6666);
    udp_video_receiver.bind(QHostAddress::Any, 6667);

    connect(&udp_audio_receiver, SIGNAL(readyRead()), this, SLOT(udp_audio_recv_slot()));
    connect(&udp_video_receiver, SIGNAL(readyRead()), this, SLOT(udp_video_recv_slot()));

    QAudioFormat fmt;
    fmt.setSampleRate(8000);
    fmt.setChannelCount(1);
    fmt.setSampleSize(16);
    fmt.setCodec("audio/pcm");
    fmt.setSampleType(QAudioFormat::SignedInt);
    fmt.setByteOrder(QAudioFormat::LittleEndian);

    audio_out = new QAudioOutput(fmt, this);
    audio_out_dev = audio_out->start();
}

void Widget::udp_audio_recv_slot()
{
    QHostAddress sender_ip;
    quint16 sender_port;

    while (udp_audio_receiver.hasPendingDatagrams())
    {
        memset(&au, 0, sizeof(au));
        udp_audio_receiver.readDatagram((char *)&au, sizeof(au), &sender_ip, &sender_port);
        audio_out_dev->write(au.data, au.length);
    }
}

void Widget::udp_video_recv_slot()
{
    QHostAddress sender_ip;
    quint16 sender_port;

    quint64 size = udp_video_receiver.pendingDatagramSize();
    QByteArray array;
    array.resize(size);

    udp_video_receiver.readDatagram(array.data(), array.size(), &sender_ip, &sender_port);

    QByteArray temp = qUncompress(array);
    QBuffer buffer(&temp);
    QImageReader image_reader(&buffer, "JPEG");
    QImage image = image_reader.read();

    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    //ui->imageLabel->resize(image.width(), image.height());
}

