using iur_sw_airportTable.Service;
using iur_sw_airportTable.ViewModel;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security.RightsManagement;
using System.Xml.Linq;
using static System.Net.Mime.MediaTypeNames;


namespace iur_sw_airportTable.Model
{
    public enum TripStatus
    {
        SCHEDULED, DELAYED, BOARDING, CHECK_IN, LAST_CALL, CANCELLED, DEPARTED, LANDED
    }

    public class Trip
    {
        private string _terminal;
        private string _flight;
        private string _airport;
        private DateTime _date;
        private Time _time;
        private int _gate;
        private TripStatus _remark;
        private User _user;
        private bool _isDeparture;
        private bool _isActive;
        private List<TripStatus> _currStatusList;

        private readonly List<TripStatus> statusDeparture = new List<TripStatus>()
        {
            TripStatus.SCHEDULED, 
            TripStatus.DELAYED, 
            TripStatus.BOARDING, 
            TripStatus.CHECK_IN, 
            TripStatus.LAST_CALL,
            TripStatus.CANCELLED,
            TripStatus.DEPARTED,
        };

        private readonly List<TripStatus> statusArrival = new List<TripStatus>()
        {
            TripStatus.SCHEDULED,
            TripStatus.CANCELLED,
            TripStatus.DEPARTED,
            TripStatus.LANDED
        };

        private readonly List<string> terminals = new List<string>() 
        { 
            "A", "B", "C", "D", "E", "F" 
        };

        private readonly List<string> capitals = new List<string>() 
        {
            "Paris", "Hamburg", "Amsterdam", "Athens", "Berlin", "Bratislava", "Brussels", "Budapest", "Helsinki", "London", "Luxembourg", "Madrid", "Monaco", "Oslo", "Riga", "Rome", "Sofia", "Stockholm", "Tallinn", "Tirana", "Vienna", "Warsaw" 
        };

        private readonly int gateCount = 13;


        public Trip(Random rnd, bool isDeparture, User user)
        {
            Terminal = terminals[rnd.Next(terminals.Count)];
            Airport = capitals[rnd.Next(capitals.Count)];
            Gate = rnd.Next(gateCount);
            Flight = ((char)rnd.Next(65, 89)).ToString() + ((char)rnd.Next(65, 89)).ToString() + rnd.Next(500).ToString();
            Date = DateTime.Today;
            Time = new Time(rnd);
            IsDeparture = isDeparture;
            Remark = CurrentStatusList[rnd.Next(CurrentStatusList.Count)];
            User = user;
            IsActive = true;
        }

        public Trip(User user)
        {
            Terminal = "#terminal";
            Flight = "#flight";
            Airport = "#airport";
            Date = DateTime.Today;
            Gate = -1;
            User = user;
            IsDeparture = true;
            CurrentStatusList = statusDeparture;
            Remark = _currStatusList[1];
            Time = new Time("00", "00", "AM");
            IsActive = false;
        }

        #region Properties
        public string Terminal
        {
            get { return _terminal; }
            set { _terminal = value; }
        }

        public string Flight
        {
            get { return _flight; }
            set { _flight = value; }
        }

        public string Airport
        {
            get { return _airport; }
            set { _airport = value; }
        }

        public DateTime Date
        {
            get { return _date; }
            set { _date = value; }
        }

        public Time Time
        {
            get { return _time; }
            set { _time = value; }
        }

        public int Gate
        {
            get { return _gate; }
            set { _gate = value; }
        }

        public TripStatus Remark
        {
            get { return _remark; }
            set { _remark = value; }
        }

        public User User
        {
            get { return _user; }
            set { _user = value; }
        }

        public bool IsDeparture
        {
            get { return _isDeparture; }
            set
            {
                _isDeparture = value;
                CurrentStatusList = IsDeparture ? statusDeparture : statusArrival;
                Remark = CurrentStatusList[0];
            }
        }
        public bool IsActive
        {
            get { return _isActive; }
            set
            {
                _isActive = value;
            }
        }

        public bool IsToday
        {
            get { return Date == DateTime.Today; }
            set { }
        }

        public List<TripStatus> CurrentStatusList
        {
            get { return IsDeparture ? statusDeparture : statusArrival; }
            set { _currStatusList = value; }
        }

        #endregion
    }
}
