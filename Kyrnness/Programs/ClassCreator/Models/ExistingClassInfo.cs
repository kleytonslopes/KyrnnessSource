using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ClassCreator.Models
{
    public class ExistingClassInfo
    {
        public string Name { get; set; }
        public string Path { get; set; }
        public string Include { get; set; }
        public List<string> Files { get; set; }
    }
}
