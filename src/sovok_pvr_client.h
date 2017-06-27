/*
 *
 *   Copyright (C) 2017 Sergey Shramchenko
 *   https://github.com/srg70/pvr.puzzle.tv
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

#include "xbmc_pvr_types.h"
#include <string>
#include "addon.h"

class CHelper_libXBMC_pvr;
class InputBuffer;
class SovokTV;

class SovokPVRClient: public IPvrIptvDataSource
{
public:
    ADDON_STATUS Init(ADDON::CHelper_libXBMC_addon *addonHelper, CHelper_libXBMC_pvr *pvrHelper,  PVR_PROPERTIES* pvrprops);
    ~SovokPVRClient();

    int GetSettings(ADDON_StructSetting ***sSet);
    ADDON_STATUS SetSetting(const char *settingName, const void *settingValue);
    void FreeSettings();
    
    //    virtual const char *GetBackendName(void);
    PVR_ERROR GetAddonCapabilities(PVR_ADDON_CAPABILITIES* pCapabilities);
    bool CanPauseStream();
    bool CanSeekStream();

    PVR_ERROR SignalStatus(PVR_SIGNAL_STATUS &signalStatus);
    ADDON_STATUS GetStatus();

    
    PVR_ERROR GetEPGForChannel(ADDON_HANDLE handle, const PVR_CHANNEL& channel, time_t iStart, time_t iEnd);

    int GetChannelGroupsAmount();
    PVR_ERROR GetChannelGroups(ADDON_HANDLE handle, bool bRadio);
    PVR_ERROR GetChannelGroupMembers(ADDON_HANDLE handle, const PVR_CHANNEL_GROUP& group);

    int GetChannelsAmount();
    PVR_ERROR GetChannels(ADDON_HANDLE handle, bool bRadio);

    PVR_ERROR  MenuHook(const PVR_MENUHOOK &menuhook, const PVR_MENUHOOK_DATA &item);
    
    bool OpenLiveStream(const PVR_CHANNEL& channel);
    void CloseLiveStream();
    int ReadLiveStream(unsigned char* pBuffer, unsigned int iBufferSize);
    long long SeekLiveStream(long long iPosition, int iWhence);
    long long PositionLiveStream();
    long long LengthLiveStream();

    bool SwitchChannel(const PVR_CHANNEL& channel);

    void SetTimeshiftEnabled(bool enable);
    bool IsTimeshiftEnabled() { return m_isTimeshiftEnabled; }
    void SetTimeshiftPath(const std::string& path);

    void SetAddFavoritesGroup(bool shouldAddFavoritesGroup);
    bool ShouldAddFavoritesGroup() { return m_shouldAddFavoritesGroup; }

    void SetStreamerId(int streamerIdx);
    int GetStreamerId();
//    const StreamerNamesList& GetStreamersList();

    void SetPinCode(const std::string& code);
    int GetRecordingsAmount(bool deleted);
    PVR_ERROR GetRecordings(ADDON_HANDLE handle, bool deleted);
    bool OpenRecordedStream(const PVR_RECORDING &recording);
    void CloseRecordedStream(void);
    int ReadRecordedStream(unsigned char *pBuffer, unsigned int iBufferSize);
    long long SeekRecordedStream(long long iPosition, int iWhence);
    long long PositionRecordedStream(void);
    long long LengthRecordedStream(void);

    PVR_ERROR CallMenuHook(const PVR_MENUHOOK &menuhook, const PVR_MENUHOOK_DATA &item);


private:
    void CreateCore();
    
    SovokTV* m_sovokTV;
    ADDON::CHelper_libXBMC_addon *m_addonHelper;
    CHelper_libXBMC_pvr *m_pvrHelper;
    std::string m_currentURL;
    InputBuffer *m_inputBuffer;
    InputBuffer *m_recordBuffer;
    bool m_isTimeshiftEnabled;
    bool m_shouldAddFavoritesGroup;
    std::string m_CacheDir;
    std::string m_login;
    std::string m_password;
    std::string m_strimmer;
    std::string m_clientPath;
    std::string m_userPath;
    bool m_enableAdult;
};
