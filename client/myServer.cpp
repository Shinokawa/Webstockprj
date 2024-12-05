#include "myServer.h"
#include <QUrl>
#include <QCoreApplication>
#include <QEventLoop>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QByteArray>
#include <QJsonDocument>

myServer::myServer(const userManager &user): user(user){
	m_manager = new QNetworkAccessManager();
}

string myServer::PostFromURL(const string &url, const string &data) {
	auto qurl = QString::fromStdString(url);
	auto qdata = QString::fromStdString(data);
	assert(!qurl.isEmpty());

	const QUrl url1 = QUrl::fromUserInput(qurl);
	assert(url1.isValid());

	QNetworkRequest qnr(url1);
	QNetworkReply* reply = m_manager->post(qnr,qdata.toUtf8()); //m_qnam是QNetworkAccessManager对象

	QEventLoop eventLoop;
	connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
	eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

	QByteArray replyData = reply->readAll();
	reply->deleteLater();
	reply = nullptr;

	return replyData.toStdString();
}

string myServer::GetFromURL(const string &url) {
	auto qurl = QString::fromStdString(url);
 	assert(!qurl.isEmpty());

	const QUrl url1 = QUrl::fromUserInput(qurl);
 	assert(url1.isValid());

	QNetworkRequest qnr(url1);
 	QNetworkReply* reply = m_manager->get(qnr); //m_qnam是QNetworkAccessManager对象

	QEventLoop eventLoop;
 	connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
 	eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

	QByteArray replyData = reply->readAll();
 	reply->deleteLater();
 	reply = nullptr;

	return replyData.toStdString();
}

string myServer::GetChatData() {
	string url = "127.0.0.1/8786";
	auto data = GetFromURL(url);
	return data;
}

void myServer::PostMessage(const QJsonObject &data) {
	string url = "http://127.0.0.1:8786/message";
	QJsonDocument doc(data);
	PostFromURL(url,doc.toJson(QJsonDocument::Indented).toStdString());
}

void myServer::PostStarInfo(const QJsonObject &starInfo) {
	string PostURL = "http://127.0.0.1:8786/subscribe";
	QJsonDocument doc(starInfo);
	auto infoString = doc.toJson(QJsonDocument::Indented).toStdString();
	/*
	string PostData = R"({
        "uuid" : "user-1234",
        "instruments" : ["m2501", "m2502", "m2503","m2504","m2505", "m2506", "m2507","m2508"];
    })";
    */
	PostFromURL(PostURL, infoString);
}

string myServer::GetStarInfo() {
	string url = "http://127.0.0.1:8786/marketdata?uuid=user-1234";
	auto data = GetFromURL(url);
	return data;
}

void myServer::PostSetEmail(const string &EmailAdress) {
	string PostURL = "http://127.0.0.1:8786/set_email";
	PostFromURL(PostURL, EmailAdress);
}