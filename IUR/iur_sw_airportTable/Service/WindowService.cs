namespace iur_sw_airportTable.Service
{
    public interface IWindowService
    {
        void ShowWindow(object windowContent);
    }
    public class WindowService : IWindowService
    {
        public void ShowWindow(object windowContent)
        {
            var window = new MainWindow()
            {
                Content = windowContent,
            };
            window.Show();
        }
    }
}
