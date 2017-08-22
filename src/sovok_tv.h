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

#ifndef sovok_tv_h
#define sovok_tv_h

#include "libXBMC_pvr.h"
#include <string>
#include <map>
#include <set>
#include <vector>
#include <functional>
#include <list>
#include <memory>
#include "ActionQueue.hpp"

class HttpEngine;

typedef unsigned int SovokChannelId;

struct SovokChannel
{
    SovokChannelId Id;
    std::string Name;
    std::string IconPath;
    bool IsRadio;
    bool HasArchive;

    bool operator <(const SovokChannel &anotherChannel) const
    {
        return Id < anotherChannel.Id;
    }
};


struct SovokGroup
{
    std::string Name;
    std::set<SovokChannelId> Channels;
};

//struct SovokEpgCaheEntry
//{
//    int ChannelId;
//    unsigned int UniqueBroadcastId;
//    time_t StartTime;
//    time_t EndTime;
//};
struct SovokEpgEntry
{
    SovokEpgEntry()
    : ChannelId(-1)
    , HasArchive(false)
    {}
    
    SovokChannelId ChannelId;
    time_t StartTime;
    time_t EndTime;

    std::string Title;
    std::string Description;
    
    bool HasArchive;
    
    template <class T>
    void Serialize(T& writer) const
    {
        writer.StartObject();               // Between StartObject()/EndObject(),
        writer.Key("ChannelId");
        writer.Uint(ChannelId);
        writer.Key("StartTime");
        writer.Int64(StartTime);
        writer.Key("EndTime");
        writer.Int64(EndTime);
        writer.Key("Title");
        writer.String(Title.c_str());
        writer.Key("Description");
        writer.String(Description.c_str());
        writer.EndObject();
        // NOTE: do not serialize HasArchive. It'll be calculated on startup
    }
    template <class T>
    void Deserialize(T& reader)
    {
        ChannelId = reader["ChannelId"].GetUint();
        StartTime = reader["StartTime"].GetInt64();
        EndTime = reader["EndTime"].GetInt64();
        Title = reader["Title"].GetString();
        Description = reader["Description"].GetString();
        // NOTE: do not serialize HasArchive. It'll be calculated on startup
    }
};

typedef unsigned int UniqueBroadcastIdType;
typedef UniqueBroadcastIdType  SovokArchiveEntry;

struct SovokEpgCaheEntry
{
    SovokEpgCaheEntry(int channelId, time_t startTime)
        : ChannelId(channelId)
        , StartTime(startTime)
    {}

    const SovokChannelId ChannelId;
    const time_t StartTime;
};

typedef std::map<SovokChannelId, SovokChannel> ChannelList;
typedef std::map<int, SovokGroup> GroupList;
typedef std::map<UniqueBroadcastIdType, SovokEpgEntry> EpgEntryList;
//typedef std::vector<SovokEpgCaheEntry> EpgCache;
typedef std::map<std::string, std::string> ParamList;
typedef std::set<SovokChannelId> FavoriteList;
typedef std::vector<std::string> StreamerNamesList;
typedef std::map<SovokChannelId, int> SovokArchivesInfo;

class SovokExceptionBase : public std::exception
{
public:
    const char* what() const noexcept {return reason.c_str();}
    const std::string reason;
    SovokExceptionBase(const std::string& r) : reason(r) {}
    SovokExceptionBase(const char* r = "") : reason(r) {}

};

class AuthFailedException : public SovokExceptionBase
{
};

class BadSessionIdException : public SovokExceptionBase
{
public:
    BadSessionIdException() : SovokExceptionBase("Session ID es empty.") {}
};

class UnknownStreamerIdException : public SovokExceptionBase
{
public:
    UnknownStreamerIdException() : SovokExceptionBase("Unknown streamer ID.") {}
};

class MissingApiException : public SovokExceptionBase
{
public:
    MissingApiException(const char* r) : SovokExceptionBase(r) {}
};

class JsonParserException : public SovokExceptionBase
{
public:
    JsonParserException(const std::string& r) : SovokExceptionBase(r) {}
    JsonParserException(const char* r) : SovokExceptionBase(r) {}
};

class ServerErrorException : public SovokExceptionBase
{
public:
    ServerErrorException(const char* r, int c) : SovokExceptionBase(r), code(c) {}
    const int code;
};



class SovokTV
{
public:
    SovokTV(ADDON::CHelper_libXBMC_addon *addonHelper, CHelper_libXBMC_pvr *pvrHelper, const std::string &login, const std::string &password);
    ~SovokTV();

    const ChannelList &GetChannelList();
    const EpgEntryList& GetEpgList() const;
    const StreamerNamesList& GetStreamersList() const;
    
    void ForEach(std::function<void(const SovokArchiveEntry&)>& action) const;
    
    bool StartArchivePollingWithCompletion(std::function<void(void)> action);

    
    //EpgEntryList GetEpg(int channelId, time_t day);
    void  GetEpg(SovokChannelId channelId, time_t startTime, time_t endTime, EpgEntryList& epgEntries);
    bool FindEpg(SovokChannelId brodcastId, SovokEpgEntry& epgEntry);
    std::string GetArchiveForEpg(const SovokEpgEntry& epgEntry);

    const GroupList &GetGroupList();
    std::string GetUrl(SovokChannelId channelId);
    FavoriteList GetFavorites();

    int GetSreamerId() const { return m_streammerId; }
    void SetStreamerId(int streamerId);
    
    void SetPinCode(const std::string& code) {m_pinCode = code;}

private:
    typedef std::vector<std::string> StreamerIdsList;
    typedef std::function<void(const CActionQueue::ActionResult&)> TApiCallCompletion;
    
    struct ApiFunctionData;
    class HelperThread;
    
    std::string GetArchive(SovokChannelId channelId, time_t startTime);
    
    void  GetEpgForAllChannelsForNHours(time_t startTime, short numberOfHours);
    void GetEpgForAllChannels(time_t startTime, time_t endTime);
    void AddEpgEntry(UniqueBroadcastIdType id, SovokEpgEntry& entry);
    void UpdateHasArchive(SovokEpgEntry& entry) const;

    bool Login(bool wait);
    void Logout();
    void Cleanup();
    
    template <typename TParser>
    void CallApiFunction(const ApiFunctionData& data, TParser parser);
    template <typename TParser>
    void CallApiAsync(const ApiFunctionData& data, TParser parser, TApiCallCompletion completion);
    
    void BuildChannelAndGroupList();
    void LoadSettings();
    void InitArchivesInfo();
    bool LoadStreamers();
    void Log(const char* massage) const;

    void LoadEpgCache();
    void SaveEpgCache();

    template <typename TParser>
    void ParseJson(const std::string& response, TParser parser);

    
    ADDON::CHelper_libXBMC_addon *m_addonHelper;
    CHelper_libXBMC_pvr *m_pvrHelper;

    std::string m_login;
    std::string m_password;
    ChannelList m_channelList;
    SovokArchivesInfo  m_archivesInfo;
    GroupList m_groupList;
    EpgEntryList m_epgEntries;
    //time_t m_lastEpgRequestStartTime;
    time_t m_lastEpgRequestEndTime;
    int m_streammerId;
    long m_serverTimeShift;
    StreamerNamesList m_streamerNames;
    StreamerIdsList m_streamerIds;
    mutable P8PLATFORM::CMutex m_epgAccessMutex;
    HelperThread* m_archiveRefresher;
    std::string m_pinCode;
    HttpEngine* m_httpEngine;
};

#endif //sovok_tv_h
