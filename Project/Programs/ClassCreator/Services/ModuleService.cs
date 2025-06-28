using ClassCreator.Models;
using System.IO;
using System.Text.Json;

namespace ClassCreator.Services
{
    public static class ModuleService
    {
        public static List<ModuleInfo> LoadModules(string configPath)
        {
            if (!File.Exists(configPath))
                return new List<ModuleInfo>();

            string json = File.ReadAllText(configPath);
            return JsonSerializer.Deserialize<List<ModuleInfo>>(json);
        }
    }
}
