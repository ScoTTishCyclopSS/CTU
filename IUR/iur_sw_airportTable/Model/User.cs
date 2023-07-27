using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;

namespace iur_sw_airportTable.Model
{
    public class User
    {
        private string login;
        private string password;
        private string name;
        private string surname;
        private bool isAdmin;
        private BitmapImage selectedAvatar;
        private List<BitmapImage> _avatars = new List<BitmapImage>();

        public User(string _name, string _surname, string _login, string _password, bool _isAdmin, int avatar)
        {
            name = _name;
            surname = _surname;
            login = _login;
            password = _password;
            isAdmin = _isAdmin;
            Avatars = LoadAvatars();
            if (avatar < 0)
                selectedAvatar = Avatars[0];
            else if (avatar > Avatars.Count)
                selectedAvatar = Avatars[-1];
            else
                selectedAvatar = Avatars[avatar];

        }
        
        public User()
        {
            name = "#name";
            surname = "#surname";
            login = "login";
            password = "password";
            isAdmin = false;
            Avatars = LoadAvatars();
            selectedAvatar = Avatars[0];
        }

        public List<BitmapImage> Avatars
        {
            get { return _avatars; }
            set
            {
                _avatars = value;
            }
        }

        private List<BitmapImage> LoadAvatars()
        {
            List<BitmapImage> res = new List<BitmapImage>();
            Trace.WriteLine("Loading avatars...");

            // lets pretend, that here is foreach file loader...
            res.Add(new BitmapImage(new Uri("pack://application:,,,/_Resources/Avatars/avatar1.png")));
            res.Add(new BitmapImage(new Uri("pack://application:,,,/_Resources/Avatars/avatar2.png")));
            res.Add(new BitmapImage(new Uri("pack://application:,,,/_Resources/Avatars/avatar3.png")));
            res.Add(new BitmapImage(new Uri("pack://application:,,,/_Resources/Avatars/avatar4.png")));
            res.Add(new BitmapImage(new Uri("pack://application:,,,/_Resources/Avatars/avatar5.png")));
            res.Add(new BitmapImage(new Uri("pack://application:,,,/_Resources/Avatars/avatar6.png")));
            res.Add(new BitmapImage(new Uri("pack://application:,,,/_Resources/Avatars/avatar7.png")));
            res.Add(new BitmapImage(new Uri("pack://application:,,,/_Resources/Avatars/avatar8.png")));
            res.Add(new BitmapImage(new Uri("pack://application:,,,/_Resources/Avatars/avatar9.png")));

            Trace.WriteLine("Avatars loaded: " + res.Count);
            return res;
        }

        public override string ToString()
        {
            return Login;
        }

        #region Properties
        public string Login
        {
            get { return login; }
            set { login = value; }
        }

        public string Password
        {
            get { return password; }
            set { password = value; }
        }

        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        public string Surname
        {
            get { return surname; }
            set { surname = value; }
        }

        public bool IsAdmin
        {
            get { return isAdmin; }
            set { isAdmin = value; }
        }
        public BitmapImage SelectedAvatar
        {
            get { return selectedAvatar; }
            set { selectedAvatar = value; }
        }

        #endregion
    }
}
