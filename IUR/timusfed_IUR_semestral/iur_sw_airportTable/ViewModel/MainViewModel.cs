using iur_sw_airportTable.Control;
using iur_sw_airportTable.Model;
using iur_sw_airportTable.Service;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Windows.Documents;

namespace iur_sw_airportTable.ViewModel
{
    public class MainViewModel : ViewModelBase
    {
        public ViewModelBase CurrentViewModel => Navigator.CurrentViewModel; // delegate
        public ViewModelBase CurrentModalViewModel => Navigator.CurrentModalViewModel; // delegate

        public MainViewModel()
        {
            #region Application data
            ObservableCollection<User> users = new ObservableCollection<User>()
            {
                new User("Fedor", "Timushev", "timusfed", "12345", true, 3),
                new User("Andrew", "Brown", "andrewbr", "12345", true, 6),
                new User("Tom", "Rodge", "tomrodge", "12345", false, 8),
                new User("Chrissy", "Loraine", "chrilor", "12345", true, 7),
                new User("Karen", "Barb", "barbkar", "12345", false, 1),
            };

            ObservableCollection<Trip> trips = GenerateTrips(20, users);
            #endregion

            // create navigator
            Navigator = new Navigator();

            // change navigator view to demoTrips, which will invoke propertychanged here
            Navigator.CurrentViewModel = new DemoTripsViewModel(trips, users, Navigator);
            //Navigator.CurrentUser = users[0];

            // subscribe to event, AT THE END!!!
            Navigator.ViewModelChanged += ViewModelChanged;
            Navigator.ModalViewModelChanged += ModalViewModelChanged;   
        }

        private ObservableCollection<Trip> GenerateTrips(int count, ObservableCollection<User> users)
        {
            ObservableCollection<Trip> res = new ObservableCollection<Trip>();
            bool isDeparture = true;
            Random rnd = new Random();

            for (int i = 0; i < count; i++)
            {
                Trip randTrip = new Trip(rnd, isDeparture, users[rnd.Next(users.Count)]);
                isDeparture = !isDeparture;
                res.Add(randTrip);
            }
            return res;
        }

        public void ViewModelChanged()
        {
            OnPropertyChanged("CurrentViewModel");
        }

        private void ModalViewModelChanged()
        {
            OnPropertyChanged("CurrentModalViewModel");
        }
    }
}
