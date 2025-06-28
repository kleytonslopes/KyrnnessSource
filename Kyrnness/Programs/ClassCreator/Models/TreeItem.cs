using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ClassCreator.Models
{
    public enum TreeItemType
    {
        Folder,
        CppFile,
        HppFile,
        OtherFile
    }

    public class TreeItem
    {
        public string Name { get; set; }
        public string Path { get; set; }
        public TreeItemType Type { get; set; }
        public ObservableCollection<TreeItem> Children { get; set; } = new();

        public void LoadChildren()
        {
            if (Type != TreeItemType.Folder)
                return;

            // Carregar diretórios
            foreach (var dir in Directory.GetDirectories(Path))
            {
                var folderItem = new TreeItem
                {
                    Name = System.IO.Path.GetFileName(dir),
                    Path = dir,
                    Type = TreeItemType.Folder
                };
                folderItem.LoadChildren();
                Children.Add(folderItem);
            }

            // Carregar arquivos
            foreach (var file in Directory.GetFiles(Path))
            {
                var ext = System.IO.Path.GetExtension(file).ToLower();
                TreeItemType fileType = ext switch
                {
                    ".cpp" => TreeItemType.CppFile,
                    ".hpp" => TreeItemType.HppFile,
                    _ => TreeItemType.OtherFile,
                };

                Children.Add(new TreeItem
                {
                    Name = System.IO.Path.GetFileName(file),
                    Path = file,
                    Type = fileType
                });
            }
        }
    }
}
