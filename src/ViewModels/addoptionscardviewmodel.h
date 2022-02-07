#ifndef ADDOPTIONSCARDVIEWMODEL_H
#define ADDOPTIONSCARDVIEWMODEL_H

#include <QObject>
#include <QMap>
#include "configurationviewmodel.h"

class AddOptionsCardViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int cardMode READ cardMode WRITE setCardMode NOTIFY cardModeChanged)
    Q_PROPERTY(QString nameFieldTitle READ nameFieldTitle NOTIFY nameFieldTitleChanged)
    Q_PROPERTY(QString valueFieldTitle READ valueFieldTitle NOTIFY valueFieldTitleChanged)
    Q_PROPERTY(QString nameField READ nameField WRITE setNameField NOTIFY nameFieldChanged)
    Q_PROPERTY(QString valueField READ valueField WRITE setValueField NOTIFY valueFieldChanged)
    Q_PROPERTY(QString nameFieldErrorMessage READ nameFieldErrorMessage NOTIFY nameFieldErrorMessageChanged)
    Q_PROPERTY(QString valueFieldErrorMessage READ valueFieldErrorMessage NOTIFY valueFieldErrorMessageChanged)
    Q_PROPERTY(bool opened READ opened NOTIFY openedChanged)
    Q_PROPERTY(ConfigurationViewModel* configuration READ configuration WRITE setConfiguration NOTIFY configurationChanged)
    Q_PROPERTY(bool isValid READ isValid NOTIFY isValidChanged)

private:
    enum class OptionsCardModes {
      NotDefined = 0,
      AddAlias,
      AddMapping,
    };

    OptionsCardModes m_cardMode { OptionsCardModes::NotDefined };
    QScopedPointer<QMap<OptionsCardModes, QString>> m_fieldNameAliases { new QMap<OptionsCardModes, QString>() };
    QScopedPointer<QMap<OptionsCardModes, QString>> m_fieldValueAliases { new QMap<OptionsCardModes, QString>() };
    QString m_nameField { "" };
    QString m_valueField { "" };
    bool m_opened { false };
    ConfigurationViewModel* m_configuration { nullptr };
    bool m_isValid { true };
    QString m_nameFieldErrorMessage { "" };
    QString m_valueFieldErrorMessage { "" };

public:
    explicit AddOptionsCardViewModel(QObject *parent = nullptr);

    int cardMode() const noexcept { return static_cast<int>(m_cardMode); }
    void setCardMode(const int cardMode) noexcept;

    QString nameFieldTitle() const noexcept { return m_fieldNameAliases->value(m_cardMode); }

    QString valueFieldTitle() const noexcept { return m_fieldValueAliases->value(m_cardMode); }

    QString nameField() const noexcept { return m_nameField; }
    void setNameField(const QString& nameField) noexcept;

    QString valueField() const noexcept { return m_valueField; }
    void setValueField(const QString& valueField) noexcept;

    bool opened() const noexcept { return m_opened; }
    void setOpened(const bool& opened) noexcept;

    QString nameFieldErrorMessage() const noexcept { return m_nameFieldErrorMessage; }
    void setNameFieldErrorMessage(const QString& nameFieldErrorMessage) noexcept;

    QString valueFieldErrorMessage() const noexcept { return m_valueFieldErrorMessage; }
    void setValueFieldErrorMessage(const QString& valueFieldErrorMessage) noexcept;

    ConfigurationViewModel* configuration() const noexcept { return m_configuration; }
    void setConfiguration(const ConfigurationViewModel* configuration) noexcept;

    bool isValid() const noexcept { return m_isValid; }

    Q_INVOKABLE void openCard() noexcept;
    Q_INVOKABLE void saveCard() noexcept;
    Q_INVOKABLE void closeCard() noexcept;

private:
    void emitValidateResult(const bool result);
    void validate();

signals:
    void cardModeChanged();
    void nameFieldTitleChanged();
    void valueFieldTitleChanged();
    void nameFieldChanged();
    void valueFieldChanged();
    void openedChanged();
    void configurationChanged();
    void isValidChanged();
    void nameFieldErrorMessageChanged();
    void valueFieldErrorMessageChanged();

};

#endif // ADDOPTIONSCARDVIEWMODEL_H
