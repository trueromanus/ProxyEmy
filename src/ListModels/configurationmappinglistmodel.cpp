#include "configurationmappinglistmodel.h"

ConfigurationMappingListModel::ConfigurationMappingListModel(QObject *parent)
    : QObject{parent}
{

}

void ConfigurationMappingListModel::setup(QSharedPointer<QList<RouteMapping*>> mappings)
{
    m_mappings = mappings;
}
