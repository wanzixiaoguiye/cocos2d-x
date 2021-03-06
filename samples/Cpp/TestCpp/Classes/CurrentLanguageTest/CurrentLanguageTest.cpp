#include "CurrentLanguageTest.h"

CurrentLanguageTest::CurrentLanguageTest()
{
    LabelTTF* label = LabelTTF::create("Current language Test", "Arial", 28);
    addChild(label, 0);
    label->setPosition( Point(VisibleRect::center().x, VisibleRect::top().y-50) );

    LabelTTF *labelLanguage = LabelTTF::create("", "Arial", 20);
    labelLanguage->setPosition(VisibleRect::center());

    ccLanguageType currentLanguageType = Application::getInstance()->getCurrentLanguage();
    switch (currentLanguageType)
    {
    case kLanguageEnglish:
        labelLanguage->setString("current language is English");
        break;
    case kLanguageChinese:
        labelLanguage->setString("current language is Chinese");
        break;
    case kLanguageFrench:
        labelLanguage->setString("current language is French");
        break;
    case kLanguageGerman:
        labelLanguage->setString("current language is German");
        break;
    case kLanguageItalian:
        labelLanguage->setString("current language is Italian");
        break;
    case kLanguageRussian:
        labelLanguage->setString("current language is Russian");
        break;
    case kLanguageSpanish:
        labelLanguage->setString("current language is Spanish");
        break;
    case kLanguageKorean:
        labelLanguage->setString("current language is Korean");
        break;
    case kLanguageJapanese:
        labelLanguage->setString("current language is Japanese");
        break;
    case kLanguageHungarian:
        labelLanguage->setString("current language is Hungarian");
        break;
    case kLanguagePortuguese:
        labelLanguage->setString("current language is Portuguese");
        break;
    case kLanguageArabic:
        labelLanguage->setString("current language is Arabic");
        break;
    case kLanguageNorwegian:
        labelLanguage->setString("current language is Norwegian");
        break;
    case kLanguagePolish:
        labelLanguage->setString("current language is Polish");
        break;
    }

    addChild(labelLanguage);
}

void CurrentLanguageTestScene::runThisTest()
{
    Layer* pLayer = new CurrentLanguageTest();
    addChild(pLayer);

    Director::getInstance()->replaceScene(this);
    pLayer->release();
}
