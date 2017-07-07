/*
 *
 *   Copyright (C) 2017 Sergey Shramchenko
 *   https://github.com/srg70/pvr.puzzle.tv
 *
 *  Copyright (C) 2013 Alex Deryskyba (alex@codesnake.com)
 *  https://bitbucket.org/codesnake/pvr.sovok.tv_xbmc_addon
 *
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#if (defined(_WIN32) || defined(__WIN32__))
#include <windows.h>
#endif

#include "p8-platform/util/util.h"
#include "kodi/xbmc_addon_cpp_dll.h"

#include "libXBMC_pvr.h"
#include "timeshift_buffer.h"
#include "direct_buffer.h"
#include "puzzle_pvr_client.h"
#include "helpers.h"
#include "puzzle_tv.h"

using namespace std;
using namespace ADDON;
using namespace PuzzleEngine;

ADDON_STATUS PuzzlePVRClient::Init(CHelper_libXBMC_addon *addonHelper, CHelper_libXBMC_pvr *pvrHelper,
                               PVR_PROPERTIES* pvrprops)
{
    ADDON_STATUS retVal = ADDON_STATUS_OK;
    if(ADDON_STATUS_OK != (retVal = PVRClientBase::Init(addonHelper, pvrHelper, pvrprops)))
       return retVal;
    
//    m_shouldAddFavoritesGroup = true;
//    char buffer[1024];
    
//    if (m_addonHelper->GetSetting("puzzle_server_port", &buffer))
//        m_login = buffer;
//    if (m_addonHelper->GetSetting("password", &buffer))
//        m_password = buffer;

    int sercerPort = 54000;
    m_addonHelper->GetSetting("puzzle_server_port", &sercerPort);
    
   
    
    try
    {
        CreateCore();
        m_puzzleTV->SetServerPort(sercerPort);
    }
    catch (std::exception& ex)
    {
        m_addonHelper->QueueNotification(QUEUE_ERROR, "Failed to connect to Puzzle server.");
        retVal = ADDON_STATUS_LOST_CONNECTION;
    }
    
    //    PVR_MENUHOOK hook = {1, 30020, PVR_MENUHOOK_EPG};
    //    m_pvr->AddMenuHook(&hook);
    return retVal;

}

PuzzlePVRClient::~PuzzlePVRClient()
{
    // Probably is better to close streams before engine destruction
    CloseLiveStream();
    CloseRecordedStream();
    if(m_puzzleTV != NULL)
        SAFE_DELETE(m_puzzleTV);

}

void PuzzlePVRClient::CreateCore()
{
    if(m_puzzleTV != NULL)
        SAFE_DELETE(m_puzzleTV);
    m_puzzleTV = new PuzzleTV(m_addonHelper);
}

ADDON_STATUS PuzzlePVRClient::SetSetting(const char *settingName, const void *settingValue)
{
    if (strcmp(settingName, "puzzle_server_port") == 0)
    {
        if(m_puzzleTV)
            m_puzzleTV->SetServerPort(*(int *)(settingValue));
    }
    else {
        return PVRClientBase::SetSetting(settingName, settingValue);
    }
    return ADDON_STATUS_NEED_RESTART;
}

PVR_ERROR PuzzlePVRClient::GetAddonCapabilities(PVR_ADDON_CAPABILITIES *pCapabilities)
{
    //pCapabilities->bSupportsEPG = true;
    pCapabilities->bSupportsTV = true;
    pCapabilities->bSupportsRadio = true;
    
    pCapabilities->bSupportsChannelGroups = false;
    pCapabilities->bSupportsRecordings = false;
    
    pCapabilities->bSupportsTimers = false;
    pCapabilities->bSupportsChannelScan = false;
    pCapabilities->bHandlesDemuxing = false;
    pCapabilities->bSupportsRecordingPlayCount = false;
    pCapabilities->bSupportsLastPlayedPosition = false;
    pCapabilities->bSupportsRecordingEdl = false;
    
    return PVRClientBase::GetAddonCapabilities(pCapabilities);
}


PVR_ERROR PuzzlePVRClient::GetEPGForChannel(ADDON_HANDLE handle, const PVR_CHANNEL& channel, time_t iStart, time_t iEnd)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
    
//    EpgEntryList epgEntries;
//    m_sovokTV->GetEpg(channel.iUniqueId, iStart, iEnd, epgEntries);
//    EpgEntryList::const_iterator itEpgEntry = epgEntries.begin();
//    for (int i = 0; itEpgEntry != epgEntries.end(); ++itEpgEntry, ++i)
//    {
//        EPG_TAG tag = { 0 };
//        tag.iUniqueBroadcastId = itEpgEntry->first;
//        tag.iChannelNumber = channel.iUniqueId;
//        tag.strTitle = itEpgEntry->second.Title.c_str();
//        tag.strPlot = itEpgEntry->second.Description.c_str();
//        tag.startTime = itEpgEntry->second.StartTime;
//        tag.endTime = itEpgEntry->second.EndTime;
//        m_pvrHelper->TransferEpgEntry(handle, &tag);
//    }
//    return PVR_ERROR_NO_ERROR;
}
PVR_ERROR  PuzzlePVRClient::MenuHook(const PVR_MENUHOOK &menuhook, const PVR_MENUHOOK_DATA &item)
{
//    SovokEpgEntry epgEntry;
//    m_sovokTV->FindEpg(item.data.iEpgUid, epgEntry);
    return PVR_ERROR_NOT_IMPLEMENTED;
    
}
int PuzzlePVRClient::GetChannelGroupsAmount()
{
    return -1;
    
//    size_t numberOfGroups = m_sovokTV->GetGroupList().size();
//    if (m_shouldAddFavoritesGroup)
//        numberOfGroups++;
//    return numberOfGroups;
}

PVR_ERROR PuzzlePVRClient::GetChannelGroups(ADDON_HANDLE handle, bool bRadio)
{
    return PVR_ERROR_NOT_IMPLEMENTED;

//    if (!bRadio)
//    {
//        PVR_CHANNEL_GROUP pvrGroup = { 0 };
//        pvrGroup.bIsRadio = false;
//
//        if (m_shouldAddFavoritesGroup)
//        {
//            strncpy(pvrGroup.strGroupName, "Избранное", sizeof(pvrGroup.strGroupName));
//            m_pvrHelper->TransferChannelGroup(handle, &pvrGroup);
//        }
//
//        GroupList groups = m_sovokTV->GetGroupList();
//        GroupList::const_iterator itGroup = groups.begin();
//        for (; itGroup != groups.end(); ++itGroup)
//        {
//            strncpy(pvrGroup.strGroupName, itGroup->first.c_str(), sizeof(pvrGroup.strGroupName));
//            m_pvrHelper->TransferChannelGroup(handle, &pvrGroup);
//        }
//    }
//
//    return PVR_ERROR_NO_ERROR;
}

PVR_ERROR PuzzlePVRClient::GetChannelGroupMembers(ADDON_HANDLE handle, const PVR_CHANNEL_GROUP& group)
{
    return PVR_ERROR_NOT_IMPLEMENTED;

//    const std::string favoriteGroupName("Избранное");
//    if (m_shouldAddFavoritesGroup && group.strGroupName == favoriteGroupName)
//    {
//        const char* c_GroupName = favoriteGroupName.c_str();
//        FavoriteList favorites = m_sovokTV->GetFavorites();
//        FavoriteList::const_iterator itFavorite = favorites.begin();
//        for (; itFavorite != favorites.end(); ++itFavorite)
//        {
//            PVR_CHANNEL_GROUP_MEMBER pvrGroupMember = { 0 };
//            strncpy(pvrGroupMember.strGroupName, c_GroupName, sizeof(pvrGroupMember.strGroupName));
//            pvrGroupMember.iChannelUniqueId = *itFavorite;
//            m_pvrHelper->TransferChannelGroupMember(handle, &pvrGroupMember);
//        }
//    }
//
//    const GroupList &groups = m_sovokTV->GetGroupList();
//    GroupList::const_iterator itGroup = groups.find(group.strGroupName);
//    if (itGroup != groups.end())
//    {
//        std::set<int>::const_iterator itChannel = itGroup->second.Channels.begin();
//        for (; itChannel != itGroup->second.Channels.end(); ++itChannel)
//        {
//            if (group.strGroupName == itGroup->first)
//            {
//                PVR_CHANNEL_GROUP_MEMBER pvrGroupMember = { 0 };
//                strncpy(pvrGroupMember.strGroupName, itGroup->first.c_str(), sizeof(pvrGroupMember.strGroupName));
//                pvrGroupMember.iChannelUniqueId = *itChannel;
//                m_pvrHelper->TransferChannelGroupMember(handle, &pvrGroupMember);
//            }
//        }
//    }
//
//    return PVR_ERROR_NO_ERROR;
}

int PuzzlePVRClient::GetChannelsAmount()
{
    return -1;
    //return m_sovokTV->GetChannelList().size();
}

PVR_ERROR PuzzlePVRClient::GetChannels(ADDON_HANDLE handle, bool bRadio)
{
    const ChannelList &channels = m_puzzleTV->GetChannelList();
    ChannelList::const_iterator itChannel = channels.begin();
    for(; itChannel != channels.end(); ++itChannel)
    {
        const Channel &sovokChannel = itChannel->second;
        if (bRadio == sovokChannel.IsRadio)
        {
            PVR_CHANNEL pvrChannel = { 0 };
            pvrChannel.iUniqueId = sovokChannel.Id;
            pvrChannel.iChannelNumber = sovokChannel.Id;
            pvrChannel.bIsRadio = sovokChannel.IsRadio;
            strncpy(pvrChannel.strChannelName, sovokChannel.Name.c_str(), sizeof(pvrChannel.strChannelName));

            string iconUrl = "http://sovok.tv" + sovokChannel.IconPath;
            strncpy(pvrChannel.strIconPath, iconUrl.c_str(), sizeof(pvrChannel.strIconPath));;

            m_pvrHelper->TransferChannelEntry(handle, &pvrChannel);
        }
    }

    return PVR_ERROR_NO_ERROR;
}

bool PuzzlePVRClient::OpenLiveStream(const PVR_CHANNEL& channel)
{
    string url = m_puzzleTV->GetUrl(channel.iUniqueId);
    return PVRClientBase::OpenLiveStream(url);
}

bool PuzzlePVRClient::SwitchChannel(const PVR_CHANNEL& channel)
{
    string url = m_puzzleTV->GetUrl(channel.iUniqueId);
    return PVRClientBase::SwitchChannel(url);
}

//void PuzzlePVRClient::SetAddFavoritesGroup(bool shouldAddFavoritesGroup)
//{
//    if (shouldAddFavoritesGroup != m_shouldAddFavoritesGroup)
//    {
//        m_shouldAddFavoritesGroup = shouldAddFavoritesGroup;
//        m_pvrHelper->TriggerChannelGroupsUpdate();
//    }
//}


int PuzzlePVRClient::GetRecordingsAmount(bool deleted)
{
    return -1;
//    if(deleted)
//        return -1;
//    
//    int size = 0;
//    std::function<void(const ArchiveList&)> f = [&size](const ArchiveList& list){size = list.size();};
//    m_sovokTV->Apply(f);
//    if(size == 0)
//    {
//        std::function<void(void)> action = [=](){
//            m_pvrHelper->TriggerRecordingUpdate();
//        };
//        m_sovokTV->StartArchivePollingWithCompletion(action);
////            m_sovokTV->Apply(f);
////            if(size != 0)
////                action();
    
//    }
//    return size;
    
}
PVR_ERROR PuzzlePVRClient::GetRecordings(ADDON_HANDLE handle, bool deleted)
{
    return PVR_ERROR_NOT_IMPLEMENTED;
//    if(deleted)
//        return PVR_ERROR_NOT_IMPLEMENTED;
//    
//    PVR_ERROR result = PVR_ERROR_NO_ERROR;
//    SovokTV& sTV(*m_sovokTV);
//    CHelper_libXBMC_pvr * pvrHelper = m_pvrHelper;
//    ADDON::CHelper_libXBMC_addon * addonHelper = m_addonHelper;
//    std::function<void(const ArchiveList&)> f = [&sTV, &handle, pvrHelper, addonHelper ,&result](const ArchiveList& list){
//        for(const auto &  i :  list) {
//            try {
//                const SovokEpgEntry& epgTag = sTV.GetEpgList().at(i);
//
//                PVR_RECORDING tag = { 0 };
//    //            memset(&tag, 0, sizeof(PVR_RECORDING));
//                sprintf(tag.strRecordingId, "%d",  i);
//                strncpy(tag.strTitle, epgTag.Title.c_str(), PVR_ADDON_NAME_STRING_LENGTH - 1);
//                strncpy(tag.strPlot, epgTag.Description.c_str(), PVR_ADDON_DESC_STRING_LENGTH - 1);
//                strncpy(tag.strChannelName, sTV.GetChannelList().at(epgTag.ChannelId).Name.c_str(), PVR_ADDON_NAME_STRING_LENGTH - 1);
//                tag.recordingTime = epgTag.StartTime;
//                tag.iLifetime = 0; /* not implemented */
//
//                tag.iDuration = epgTag.EndTime - epgTag.StartTime;
//                tag.iEpgEventId = i;
//                tag.iChannelUid = epgTag.ChannelId;
//
//                string dirName = tag.strChannelName;
//                char buff[20];
//                strftime(buff, sizeof(buff), "/%d-%m-%y", localtime(&epgTag.StartTime));
//                dirName += buff;
//                strncpy(tag.strDirectory, dirName.c_str(), PVR_ADDON_NAME_STRING_LENGTH - 1);
//
//                pvrHelper->TransferRecordingEntry(handle, &tag);
//                
//            }
//            catch (...)  {
//                addonHelper->Log(LOG_ERROR, "%s: failed.", __FUNCTION__);
//                result = PVR_ERROR_FAILED;
//            }
//
//        }
//    };
//    m_sovokTV->Apply(f);
//    return result;
}

bool PuzzlePVRClient::OpenRecordedStream(const PVR_RECORDING &recording)
{
    return false;
    
//    SovokEpgEntry epgTag;
//    
//    unsigned int epgId = recording.iEpgEventId;
//    if( epgId == 0 )
//        epgId = strtoi(recording.strRecordingId);
//    if(!m_sovokTV->FindEpg(epgId, epgTag))
//        return false;
//    
//    string url = m_sovokTV->GetArchiveForEpg(epgTag);
//    return PVRClientBase::OpenRecordedStream(url);
}

PVR_ERROR PuzzlePVRClient::CallMenuHook(const PVR_MENUHOOK &menuhook, const PVR_MENUHOOK_DATA &item)
{
    m_addonHelper->Log(LOG_DEBUG, " >>>> !!!! Menu hook !!!! <<<<<");
    return MenuHook(menuhook, item);
}

PVR_ERROR PuzzlePVRClient::SignalStatus(PVR_SIGNAL_STATUS &signalStatus)
{
    snprintf(signalStatus.strAdapterName, sizeof(signalStatus.strAdapterName), "IPTV Puzzle TV Adapter 1");
    snprintf(signalStatus.strAdapterStatus, sizeof(signalStatus.strAdapterStatus), (m_puzzleTV == NULL) ? "Not connected" :"OK");
    return PVR_ERROR_NO_ERROR;
}

ADDON_STATUS PuzzlePVRClient::GetStatus(){ return  /*(m_sovokTV == NULL)? ADDON_STATUS_LOST_CONNECTION : */ADDON_STATUS_OK;}
       


