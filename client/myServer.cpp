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

/*
string myServer::PostFromURL(const string &url, const string &data) {
	// 创建 QNetworkRequest 对象并设置 URL
	QNetworkRequest request(QUrl(QString::fromStdString(url)));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	request.setHeader(QNetworkRequest::UserAgentHeader,"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/131.0.0.0 Safari/537.36 Edg/131.0.0.0");

	// 发送 POST 请求
	QNetworkReply *reply = m_manager->post(request, QString::fromStdString(data).toUtf8());

	// 使用 QEventLoop 阻塞当前线程，直到请求完成
	QEventLoop loop;
	connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	loop.exec(); // 等待请求完成

	// 处理结果
	QString response;
	if (reply->error() == QNetworkReply::NoError) {
		response = QString::fromUtf8(reply->readAll());
	}
	else {
		response = "Error: " + reply->errorString();
	}

	reply->deleteLater(); // 清理资源
	return response.toStdString();      // 返回结果
}
*/

/*
string myServer::GetFromURL(const string &url) {
	// 创建 QNetworkRequest 对象并设置 URL
	QNetworkRequest request(QUrl(QString::fromStdString(url)));
	//request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	request.setHeader(QNetworkRequest::UserAgentHeader,"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/131.0.0.0 Safari/537.36 Edg/131.0.0.0");

	// 发送 POST 请求
	QNetworkReply *reply = m_manager->get(request);

	// 使用 QEventLoop 阻塞当前线程，直到请求完成
	QEventLoop loop;
	connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	loop.exec(); // 等待请求完成

	// 处理结果
	QString response;
	if (reply->error() == QNetworkReply::NoError) {
		response = QString::fromUtf8(reply->readAll());
	}
	else {
		response = "Error: " + reply->errorString();
	}

	reply->deleteLater(); // 清理资源
	return response.toStdString();  // 返回结果
}
*/

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
	string url = "127.0.0.1/8786";
	QJsonDocument doc(data);
	PostFromURL(url,doc.toJson(QJsonDocument::Indented).toStdString());
}
