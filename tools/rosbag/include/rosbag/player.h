/*********************************************************************
* Software License Agreement (BSD License)
*
*  Copyright (c) 2008, Willow Garage, Inc.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of Willow Garage, Inc. nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
********************************************************************/

#ifndef ROSBAG_PLAYER_H
#define ROSBAG_PLAYER_H

#include <sys/stat.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include <queue>
#include <string>

#include <ros/ros.h>
#include <ros/time.h>

#include <std_msgs/Empty.h>
#include <topic_tools/shape_shifter.h>

#include "rosbag/bag.h"

namespace rosbag {

struct PlayerOptions
{
    PlayerOptions();

    void check();

    bool     check_bag;
    bool     show_defs;
    bool     quiet;
    bool     start_paused;
    bool     at_once;
    bool     bag_time;
    double   bag_time_frequency;
    double   time_scale;
    int      queue_size;
    uint64_t advertise_sleep;
    bool     try_future;
    bool     has_time;
    float    time;

    std::vector<std::string> bags;
};

//! \todo this information is already stored in the bag file
struct BagContent
{
    BagContent(std::string const& d, std::string const& m, std::string const& def);

    std::string datatype;
    std::string md5sum;
    std::string definition;
    int         count;
};

class Player
{
public:
    Player(PlayerOptions const& options);
    ~Player();

    void publish();
    int  checkBag();

private:
    char readCharFromStdin();
    void setTerminalSettings();
    void unsetTerminalSettings();

    ros::Time getSysTime();
    void      doPublish(std::string const& topic, ros::MessagePtr m, ros::Time const& time);

private:
    PlayerOptions options_;

    ros::NodeHandle* node_handle_;    //!< pointer to allow player to start before node handle exists since this is where argument parsing happens

    ros::Time start_time_;
    bool      paused_;

    std::vector<boost::shared_ptr<Bag> >  bags_;
    std::map<std::string, ros::Publisher> publishers_;

    // Terminal
    termios orig_flags_;
    fd_set  stdin_fdset_;
    int     maxfd_;
};

}

#endif
