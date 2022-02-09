#include "addoptionscardviewmodel.h"

AddOptionsCardViewModel::AddOptionsCardViewModel(QObject *parent)
    : QObject{parent}
{
    m_fieldNameAliases->insert(OptionsCardModes::NotDefined, "");
    m_fieldNameAliases->insert(OptionsCardModes::AddAlias, "Alias");
    m_fieldNameAliases->insert(OptionsCardModes::AddMapping, "Route");

    m_fieldValueAliases->insert(OptionsCardModes::NotDefined, "");
    m_fieldValueAliases->insert(OptionsCardModes::AddAlias, "Value");
    m_fieldValueAliases->insert(OptionsCardModes::AddMapping, "Target Url");

}

void AddOptionsCardViewModel::setCardMode(const int cardMode) noexcept
{
    auto cardModeEnum = static_cast<OptionsCardModes>(cardMode);
    if (m_cardMode == cardModeEnum) return;

    m_cardMode = cardModeEnum;
    emit cardModeChanged();
    emit nameFieldTitleChanged();
    emit valueFieldTitleChanged();
}

void AddOptionsCardViewModel::setNameField(const QString &nameField) noexcept
{
    if (m_nameField == nameField) return;

    m_nameField = nameField;
    emit nameFieldChanged();
    validate();
}

void AddOptionsCardViewModel::setValueField(const QString &valueField) noexcept
{
    if (m_valueField == valueField) return;

    m_valueField = valueField;
    emit valueFieldChanged();
    validate();
}

void AddOptionsCardViewModel::setOpened(const bool &opened) noexcept
{
    if (m_opened == opened) return;

    m_opened = opened;
    emit openedChanged();
}

void AddOptionsCardViewModel::setNameFieldErrorMessage(const QString &nameFieldErrorMessage) noexcept
{
    if (m_nameFieldErrorMessage == nameFieldErrorMessage) return;

    m_nameFieldErrorMessage = nameFieldErrorMessage;
    emit nameFieldErrorMessageChanged();
}

void AddOptionsCardViewModel::setValueFieldErrorMessage(const QString &valueFieldErrorMessage) noexcept
{
    if (m_valueFieldErrorMessage == valueFieldErrorMessage) return;

    m_valueFieldErrorMessage = valueFieldErrorMessage;
    emit valueFieldErrorMessageChanged();
}

void AddOptionsCardViewModel::setConfiguration(const ConfigurationViewModel *configuration) noexcept
{
    if (m_configuration != nullptr) return;

    m_configuration = const_cast<ConfigurationViewModel*>(configuration);
    emit configurationChanged();
}

void AddOptionsCardViewModel::openCard() noexcept
{
    if (m_cardMode == OptionsCardModes::NotDefined) return;

    setNameFieldErrorMessage("");
    setValueFieldErrorMessage("");

    setNameField("");
    setValueField("");

    emitValidateResult(false);

    setOpened(true);
}

void AddOptionsCardViewModel::saveCard() noexcept
{
    setNameFieldErrorMessage("");
    setValueFieldErrorMessage("");

    auto result = false;
    std::tuple<bool, QString, QString> resultTuple;

    if (m_cardMode == OptionsCardModes::AddAlias) resultTuple = m_configuration->addAlias(m_nameField, m_valueField);
    if (m_cardMode == OptionsCardModes::AddMapping) resultTuple = m_configuration->addMapping(m_nameField, m_valueField);

    result = std::get<0>(resultTuple);
    if (!result) {
        auto errorField = std::get<1>(resultTuple);
        auto errorMessage = std::get<2>(resultTuple);
        if (errorField == "name") {
            setNameFieldErrorMessage(errorMessage);
        } else {
            setValueFieldErrorMessage(errorMessage);
        }
    }

    if (result) closeCard();
}

void AddOptionsCardViewModel::closeCard() noexcept
{
    setOpened(false);
    m_cardMode = OptionsCardModes::NotDefined;
    emit cardModeChanged();
}

void AddOptionsCardViewModel::emitValidateResult(const bool result)
{
    m_isValid = result;
    emit isValidChanged();
}

void AddOptionsCardViewModel::validate()
{
    if (m_nameField.isEmpty() || m_valueField.isEmpty()) {
        emitValidateResult(false);
        return;
    }

    emitValidateResult(true);
}
