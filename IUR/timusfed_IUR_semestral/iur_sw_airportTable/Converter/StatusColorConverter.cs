using iur_sw_airportTable.Model;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Media;

namespace iur_sw_airportTable.Converter
{
    public class StatusColorConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            TripStatus status = (TripStatus)value;

            List<TripStatus> asGreen = new List<TripStatus>()
            {
                TripStatus.BOARDING, TripStatus.CHECK_IN, TripStatus.DEPARTED, TripStatus.LANDED
            };

            List<TripStatus> asYellow = new List<TripStatus>()
            {
                TripStatus.SCHEDULED, TripStatus.DELAYED
            };

            List<TripStatus> asRed = new List<TripStatus>()
            {
                TripStatus.CANCELLED, TripStatus.LAST_CALL
            };

            if (asGreen.Contains(status))
                return new SolidColorBrush(Colors.Green);

            if (asYellow.Contains(status))
                return new SolidColorBrush(Colors.Yellow);

            if (asRed.Contains(status))
                return new SolidColorBrush(Colors.Red);

            return new SolidColorBrush(Colors.White);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
