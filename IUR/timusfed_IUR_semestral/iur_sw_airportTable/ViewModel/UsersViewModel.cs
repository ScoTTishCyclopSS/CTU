using iur_sw_airportTable.Command;
using iur_sw_airportTable.Control;
using iur_sw_airportTable.Model;
using iur_sw_airportTable.Properties;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Resources;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media.Imaging;

namespace iur_sw_airportTable.ViewModel
{
    public class UsersViewModel : ViewModelBase
    {
        private ObservableCollection<User> _users;
        //________________________
        private int _selectedIndex;
        private User _selectedUser;
        //________________________
        private RelayCommand _addUserCommand;
        private RelayCommand _removeUserCommand;

        public ObservableCollection<User> Users
        {
            get { return _users; }
            set { _users = value; }
        }

        public UsersViewModel(ObservableCollection<User> users, Navigator navigator)
        {
            Users = users;
            Navigator = navigator;
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

        #region SelectedUser stuff
        public User SelectedUser
        {
            get
            {
                return _selectedUser;
            }
            set
            {
                if (_selectedUser != value)
                {
                    _selectedUser = value;
                    OnPropertyChanged("SelectedUser");
                }
            }
        }
        #endregion

        #region AddUserCommand
        public ICommand AddUserCommand
        {
            get
            {
                return _addUserCommand ?? (_addUserCommand = new RelayCommand(AddUser, SimpleOk));
            }
        }

        private void AddUser(object o)
        {
            Users.Add(new User());
            SelectedIndex = Users.Count - 1;
        }
        #endregion

        #region RemoveUserCommand
        public ICommand RemoveUserCommand
        {
            get
            {
                return _removeUserCommand ?? (_removeUserCommand = new RelayCommand(RemoveUser, UserCanBeRemoved));
            }
        }

        private void RemoveUser(object o)
        {
            Users.Remove(SelectedUser);
            SelectedIndex =- 1;
        }

        private bool UserCanBeRemoved(object o)
        {
            return SelectedIndex > -1;
        }
        #endregion
    }

    public class LogInViewModel : UsersViewModel
    {
        private RelayCommand _logInCommand;
        private RelayCommand _closeModalCommand;
        private string _errMsg;

        public LogInViewModel(ObservableCollection<User> users, Navigator navigator) : base(users, navigator)
        {
        }

        public string ErrMsg
        {
            get { return _errMsg; }
            set { _errMsg = value; OnPropertyChanged("ErrMsg"); }
        }

        #region LogInCommand
        public ICommand LogInCommand
        {
            get
            {
                return _logInCommand ?? (_logInCommand = new RelayCommand(LogIn, SimpleOk));
            }
        }

        private void LogIn(object o)
        {
            var values = (object[])o;
            var login = values[0].ToString();
            //var password = values[1].ToString();
            //for security reasons WPF doesn't provide a dependency property for the Password of PasswordBox, sad
            var password = ((PasswordBox)values[1]).Password.ToString();

            Trace.WriteLine(login + ':' + password);

            // find user in collection
            User userCriteria = null;
            try
            {
                userCriteria = Users.Single(i => i.Login == login && i.Password == password);
                Trace.WriteLine("Login ok");
                Navigator.CurrentModalViewModel = null;
                Navigator.CurrentUser = userCriteria;
            }
            catch (InvalidOperationException e)
            {
                ErrMsg = "Login or password error!";
                Trace.WriteLine("Login bruh" + e.Message);
            }
        }
        #endregion

        #region CloseModalCommand
        public ICommand CloseModalCommand
        {
            get
            {
                return _closeModalCommand ?? (_closeModalCommand = new RelayCommand(CloseModal, SimpleOk));
            }
        }

        private void CloseModal(object o)
        {
            Navigator.CurrentModalViewModel = null;
        }
        #endregion
    }

    public class UsersEditViewModel : UsersViewModel
    {
        public UsersEditViewModel(ObservableCollection<User> users, Navigator navigator) : base(users, navigator)
        {
        }
    }
}
