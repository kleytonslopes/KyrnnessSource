using ClassCreator.Models;
using ClassCreator.Services;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Runtime.CompilerServices;

namespace ClassCreator.ViewModels
{
    public class MainViewModel : INotifyPropertyChanged
    {
        public ObservableCollection<ModuleInfo> Modules { get; set; } = new();
        public ObservableCollection<TreeItem> DirectoryTree { get; set; } = new();

        private ModuleInfo selectedModule;
        public ModuleInfo SelectedModule
        {
            get => selectedModule;
            set
            {
                selectedModule = value;
                OnPropertyChanged();
                LoadDirectoryTree();
            }
        }

        private string selectedFolderPath;
        public string SelectedFolderPath
        {
            get => selectedFolderPath;
            set
            {
                selectedFolderPath = value;
                OnPropertyChanged();
            }
        }

        private string className;
        public string ClassName
        {
            get => className;
            set { className = value; OnPropertyChanged(); }
        }

        private string classPrefix;
        public string ClassPrefix
        {
            get => classPrefix;
            set { classPrefix = value; OnPropertyChanged(); }
        }

        private string baseClass;
        public string BaseClass
        {
            get => baseClass;
            set { baseClass = value; OnPropertyChanged(); }
        }

        private string namespaceName;
        public string NamespaceName
        {
            get => namespaceName;
            set { namespaceName = value; OnPropertyChanged(); }
        }

        private string exportMacro;
        public string ExportMacro
        {
            get => exportMacro;
            set { exportMacro = value; OnPropertyChanged(); }
        }

        private string selectedCopyBehavior = "Default";
        public string SelectedCopyBehavior
        {
            get => selectedCopyBehavior;
            set { selectedCopyBehavior = value; OnPropertyChanged(); }
        }

        private string selectedMoveBehavior = "Default";
        public string SelectedMoveBehavior
        {
            get => selectedMoveBehavior;
            set { selectedMoveBehavior = value; OnPropertyChanged(); }
        }

        private bool generateEquality;
        public bool GenerateEquality
        {
            get => generateEquality;
            set { generateEquality = value; OnPropertyChanged(); }
        }

        private List<ExistingClassInfo> availableBaseClasses;
        public List<ExistingClassInfo> AvailableBaseClasses
        {
            get => availableBaseClasses;
            set
            {
                availableBaseClasses = value;
                OnPropertyChanged();
            }
        }

        private ExistingClassInfo selectedBaseClass;
        public ExistingClassInfo SelectedBaseClass
        {
            get => selectedBaseClass;
            set
            {
                selectedBaseClass = value;
                OnPropertyChanged();

                // Quando o usuário escolher, atualiza o campo de herança
                if (value != null)
                {
                    BaseClass = value.Name;
                }
            }
        }

        public MainViewModel()
        {
            string configPath = "ModulesConfig.json";
            var modules = ModuleService.LoadModules(configPath);
            foreach (var module in modules)
                Modules.Add(module);

            AvailableBaseClasses = ClassDatabaseService.LoadClasses();
        }

        public void LoadDirectoryTree()
        {
            DirectoryTree.Clear();

            if (SelectedModule == null || !System.IO.Directory.Exists(SelectedModule.Path))
                return;

            // Exemplo simples de carregar TreeItems — adapte para sua implementação
            var root = new TreeItem
            {
                Name = SelectedModule.Name,
                Path = SelectedModule.Path,
                Type = TreeItemType.Folder
            };
            root.LoadChildren(); // método que você implementa para carregar os filhos recursivamente

            DirectoryTree.Add(root);
        }

        public string GetBaseClassInclude(string className)
        {
            ExistingClassInfo info = AvailableBaseClasses.FirstOrDefault(x => x.Name == className);
            if (info != null)
            {
                return $"#include \"{info.Include}\"";
            }
            return string.Empty;
        }

        public event PropertyChangedEventHandler PropertyChanged;
        private void OnPropertyChanged([CallerMemberName] string name = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }
    }
}
