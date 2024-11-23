#include <QtTest>

// add necessary includes here
#include "DatabaseManager.h"

class TestDatabaseManager : public QObject
{
    Q_OBJECT

public:
    TestDatabaseManager();
    ~TestDatabaseManager();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testDatabaseConnection();

private:
    DatabaseManager* dbManager;
};

TestDatabaseManager::TestDatabaseManager() {
    dbManager = new DatabaseManager();
}

TestDatabaseManager::~TestDatabaseManager() {
    delete dbManager; 
}

void TestDatabaseManager::initTestCase() {
    dbManager->init();
}

void TestDatabaseManager::cleanupTestCase() {
    dbManager->execute("DELETE FROM scan", QList<QVariant>());
    dbManager->execute("DELETE FROM profile", QList<QVariant>());
    dbManager->execute("DELETE FROM users", QList<QVariant>());
}

void TestDatabaseManager::testDatabaseConnection() {
    QVERIFY(dbManager->isConnectionOpen());
}

//QTEST_APPLESS_MAIN(TestDatabaseManager)
QTEST_MAIN(TestDatabaseManager)  // Runs the tests in TestDatabaseManager
#include "TestDatabaseManager.moc"


