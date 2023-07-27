using iur_sw_airportTable.Model;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace iur_sw_airportTable.Service
{

    public class Time
    {
        // WHY THERE IS NO TIME PICKER IN WPF?????
        private string _hours;
        private string _minutes;
        private string _format;

        public string Hours
        {
            get { return _hours; }
            set { _hours = value; }
        }

        public string Minutes
        {
            get { return _minutes; }
            set { _minutes = value; }
        }

        public string Format
        {
            get { return _format; }
            set { _format = value; }
        }

        private readonly List<string> timeHours = new List<string>()
        {
            "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"
        };

        private readonly List<string> timeMinutes = new List<string>()
        {
            "00", "10", "20", "30", "40", "50"
        };

        private readonly List<string> timeFormat = new List<string>()
        {
            "AM", "PM"
        };

        public List<string> HoursList
        {
            get { return timeHours; }
            set { }
        }

        public List<string> MinutesList
        {
            get { return timeMinutes; }
            set { }
        }

        public List<string> FormatList
        {
            get { return timeFormat; }
            set { }
        }

        public Time(string hours, string minutes, string format)
        {
            Hours = hours;
            Minutes = minutes;
            Format = format;
        }

        public Time(Random rnd)
        {
            RandomTime(rnd);
        }

        public override string ToString()
        {
            return Hours + ":" + Minutes + " " + Format;
        }

        private void RandomTime(Random rnd)
        {
            Hours = timeHours[rnd.Next(timeHours.Count)];
            Minutes = timeMinutes[rnd.Next(timeMinutes.Count)];
            Format = timeFormat[rnd.Next(timeFormat.Count)];
        }
    }
}
