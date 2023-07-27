using iur_sw_airportTable.Model;
using System;

namespace iur_sw_airportTable.Control
{
    public class Navigator
    {
        private ViewModelBase _currViewModel;
        private ViewModelBase _currModalViewModel;
        //________________________
        private User _currentUser;

        #region CurrentViewModel
        public ViewModelBase CurrentViewModel
        {
            get { return _currViewModel; }
            set { _currViewModel = value; OnModelViewChanged(); }
        }

        public event Action ViewModelChanged;
        private void OnModelViewChanged()
        {
            ViewModelChanged?.Invoke();
        }
        #endregion

        #region CurrentModalViewModel
        public ViewModelBase CurrentModalViewModel
        {
            get { return _currModalViewModel; }
            set { _currModalViewModel = value; OnModalModelViewChanged(); }
        }

        public event Action ModalViewModelChanged;
        private void OnModalModelViewChanged()
        {
            ModalViewModelChanged?.Invoke();
        }
        #endregion

        #region CurrentUser
        public User CurrentUser
        {
            get { return _currentUser; }
            set { _currentUser = value; OnCurrentUserChanged(); }
        }

        public bool IsLogged => CurrentUser != null;

        public event Action CurrentUserChanged;
        private void OnCurrentUserChanged()
        {
            CurrentUserChanged?.Invoke();
        }
        #endregion
    }
}
