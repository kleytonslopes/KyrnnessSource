using ClassCreator.Models;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

namespace ClassCreator.Services
{
    public static class ClassDatabaseService
    {
        private static readonly string DatabasePath = "ClassesDatabase.json";

        public static List<ExistingClassInfo> LoadClasses()
        {
            if (!File.Exists(DatabasePath))
                return new List<ExistingClassInfo>();

            string json = File.ReadAllText(DatabasePath);
            return JsonSerializer.Deserialize<List<ExistingClassInfo>>(json);
        }
    }
}
