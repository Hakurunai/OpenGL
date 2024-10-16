using Eto.Forms;
using OpenTK.Graphics.OpenGL;
using OpenTK.Windowing.Common;
using OpenTK.Windowing.Desktop;
using System;

namespace LPrep
{
    internal class CheckOpenGL
    {
        private readonly Application app;

        public CheckOpenGL(Application application)
        {
            app = application;
            CheckOpenGLVersion(); // Directly call the check on the main thread
        }

        private void CheckOpenGLVersion()
        {
            try
            {
                // Set up OpenTK's GameWindow
                var gameWindowSettings = new GameWindowSettings();
                var nativeWindowSettings = new NativeWindowSettings()
                {
                    StartVisible = false,
                    API = ContextAPI.OpenGL,
                    APIVersion = new Version(4, 6)
                };

                using (var window = new GameWindow(gameWindowSettings, nativeWindowSettings))
                {
                    window.MakeCurrent(); // Make the context current
                    Console.WriteLine("OpenGL context created successfully.");

                    // Get the installed OpenGL version
                    string glVersion = GL.GetString(StringName.Version);
                    Console.WriteLine($"Detected OpenGL Version: {glVersion}");

                    var versionParts = glVersion.Split('.');
                    if (versionParts.Length >= 2)
                    {
                        int major = int.Parse(versionParts[0]);
                        int minor = int.Parse(versionParts[1]);

                        app.Invoke(() => // Ensure the message box is shown on the main thread
                        {
                            if (major > 4 || (major == 4 && minor >= 6))
                            {
                                MessageBox.Show("Your system supports OpenGL 4.6 or higher. All is good.", MessageBoxType.Information);
                            }
                            else
                            {
                                MessageBox.Show("Your system does not support OpenGL 4.6 or higher. Please, update your drivers before launching LightBolt.", MessageBoxType.Warning);
                            }
                            app.Quit();
                        });
                    }
                    else
                    {
                        app.Invoke(() => // Ensure the message box is shown on the main thread
                        {
                            MessageBox.Show("Failed to detect OpenGL version.", MessageBoxType.Error);
                            app.Quit();
                        });
                    }
                }
            }
            catch (Exception ex)
            {
                // Log the error message
                Console.WriteLine($"Error during OpenGL check: {ex.Message}");
                app.Invoke(() => // Ensure the message box is shown on the main thread
                {
                    MessageBox.Show($"An error occurred: {ex.Message}", MessageBoxType.Error);
                });
            }
        }
    }
}