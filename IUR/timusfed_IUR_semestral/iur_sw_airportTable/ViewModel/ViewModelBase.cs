using iur_sw_airportTable.Control;
using iur_sw_airportTable.Model;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public class ViewModelBase : INotifyPropertyChanged
{
    public Navigator Navigator { get; set; }
    public User CurrentUser => Navigator.CurrentUser;
    public bool IsLogged => Navigator.IsLogged;
    public bool IsNotReadOnly => !Navigator.IsLogged;

    #region INotifyPropertyChanged stuff
    public event PropertyChangedEventHandler PropertyChanged;
    protected void OnPropertyChanged(string property)
    {
        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(property));
    }

    public bool SimpleOk(object o)
    {
        return true;
    }
    #endregion
}
