using iur_sw_airportTable.Command;
using iur_sw_airportTable.Control;
using iur_sw_airportTable.Model;
using iur_sw_airportTable.Service;
using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Windows.Input;
using System.Windows.Threading;

namespace iur_sw_airportTable.ViewModel
{
    public class TripsViewModel : ViewModelBase
    {

        private ObservableCollection<Trip> _trips;
        private ObservableCollection<User> _users;
        //________________________
        private int _selectedIndex;
        private Trip _selectedTrip;
        //________________________
        private RelayCommand _addTripCommand;
        private RelayCommand _removeUserCommand;
        //________________________
        private DispatcherTimer _timer;
        private string _currTime;

        public ObservableCollection<Trip> SortedTrips
        {
            get
            {
                return new ObservableCollection<Trip>(_trips.OrderBy(t => t.Time.Format).ThenBy(t => t.Time.ToString()));
            }
        }

        public ObservableCollection<Trip> Trips
        {
            get {
                return _trips;
            }
            set { 
                _trips = value; OnPropertyChanged("Trips"); 
            }
        }
        public ObservableCollection<User> Users
        {
            get { return _users; }
            set { _users = value; }
        }

        public TripsViewModel(ObservableCollection<Trip> trips, ObservableCollection<User> users, Navigator navigator)
        {
            Trips = trips;
            Users = users;
            Navigator = navigator;
            SelectedIndex = -1;
            StartTime();
        }

        public string CurrentTime
        {
            get
            {
                return _currTime;
            }
            set
            {
                if (_currTime == value)
                    return;
                _currTime = value;
                OnPropertyChanged("CurrentTime");
            }
        }

        private void StartTime()
        {
            // timer (on render time)
            Random rnd = new Random();
            _timer = new DispatcherTimer(DispatcherPriority.Render);
            _timer.Interval = TimeSpan.FromSeconds(1);
            _timer.Tick += (sender, args) =>
            {
                CurrentTime = DateTime.Now.ToLongTimeString();
            };
            _timer.Start();
        }

        #region SelectedIndex stuff
        public int SelectedIndex
        {
            get
            {
                return _selectedIndex;
            }
            set
            {
                if (_selectedIndex != value)
                {
                    _selectedIndex = value;
                    OnPropertyChanged("SelectedIndex");
                }
            }
        }
        #endregion

        #region SelectedTrip stuff
        public Trip SelectedTrip
        {
            get
            {
                return _selectedTrip;
            }
            set
            {
                if (_selectedTrip != value)
                {
                    _selectedTrip = value;
                    OnPropertyChanged("SelectedTrip");
                }
            }
        }
        #endregion

        #region AddTripCommand        
        public ICommand AddTripCommand
        {
            get
            {
                return _addTripCommand ?? (_addTripCommand = new RelayCommand(AddTrip, SimpleOk));
            }
        }

        private void AddTrip(object o)
        {
            Trips.Add(new Trip(CurrentUser));
            SelectedIndex = Trips.Count - 1;
        }
        #endregion

        #region RemoveTripCommand
        public ICommand RemoveTripCommand
        {
            get
            {
                return _removeUserCommand ?? (_removeUserCommand = new RelayCommand(RemoveTrip, TripCanBeRemoved));
            }
        }

        private void RemoveTrip(object o)
        {
            Trips.Remove(SelectedTrip);
            SelectedIndex = -1;
        }

        private bool TripCanBeRemoved(object o)
        {
            return SelectedIndex > -1;
        }
        #endregion
    }

    public class EditTripsViewModel : TripsViewModel
    {
        private readonly WindowService _windowService;
        //________________________
        private RelayCommand _returnHomeCommand;
        private RelayCommand _editUsersCommand;

        public EditTripsViewModel(ObservableCollection<Trip> trips, ObservableCollection<User> users, Navigator navigator) : base(trips, users, navigator)
        {
            _windowService = new WindowService();
        }

        #region GoHomeCommand
        public ICommand ReturnHomeCommand
        {
            get
            {
                return _returnHomeCommand ?? (_returnHomeCommand = new RelayCommand(ReturnHome, SimpleOk));
            }
        }

        public virtual void ReturnHome(object o)
        {
            Trace.WriteLine("Go home!");
            Navigator.CurrentViewModel = new DemoTripsViewModel(Trips, Users, Navigator);
        }
        #endregion

        #region EditUsersCommand
        public ICommand EditUsersCommand
        {
            get
            {
                return _editUsersCommand ?? (_editUsersCommand = new RelayCommand(OpenUsersEditWindow, SimpleOk));
            }
        }

        private void OpenUsersEditWindow(object o)
        {
            Trace.WriteLine("Let's add some new members..");
            _windowService.ShowWindow(new UsersEditViewModel(Users, Navigator));
        }
        #endregion
    }

    public class DemoTripsViewModel : TripsViewModel
    {
        private RelayCommand _logOutCommand;
        private RelayCommand _openLogInCommand;
        private RelayCommand _openEditTripsCommand;

        public DemoTripsViewModel(ObservableCollection<Trip> trips, ObservableCollection<User> users, Navigator navigator) : base(trips, users, navigator)
        {
            Navigator.CurrentUserChanged += CurrentUserChanged; // subscribe to event
        }

        public void CurrentUserChanged()
        {
            OnPropertyChanged("IsLogged");
            OnPropertyChanged("CurrentUser");
            OnPropertyChanged("IsNotReadOnly");
        }

        #region OpenLogInCommand
        public ICommand OpenLogInCommand
        {
            get
            {
                return _openLogInCommand ?? (_openLogInCommand = new RelayCommand(OpenLogIn, SimpleOk));
            }
        }

        public virtual void OpenLogIn(object o)
        {
            if (CurrentUser == null)
            {
                Navigator.CurrentModalViewModel = new LogInViewModel(Users, Navigator);
            }
        }
        #endregion

        #region LogOutCommand
        public ICommand LogOutCommand
        {
            get
            {
                return _logOutCommand ?? (_logOutCommand = new RelayCommand(LogOut, SimpleOk));
            }
        }

        public virtual void LogOut(object o)
        {
            Trace.WriteLine("Logout...");
            Navigator.CurrentUser = null;
        }
        #endregion

        #region OpenEditorCommand
        public ICommand OpenTripsEditCommand
        {
            get
            {
                return _openEditTripsCommand ?? (_openEditTripsCommand = new RelayCommand(OpenTripsEdit, SimpleOk));
            }
        }

        public virtual void OpenTripsEdit(object o)
        {
            if (CurrentUser != null)
                Navigator.CurrentViewModel = new EditTripsViewModel(Trips, Users, Navigator);
        }
        #endregion
    }
}
