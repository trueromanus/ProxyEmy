#include "requestslogviewmodel.h"

RequestsLogViewModel::RequestsLogViewModel(QObject *parent)
    : QObject{parent}
{
    m_listModel->setup(m_items);
}
