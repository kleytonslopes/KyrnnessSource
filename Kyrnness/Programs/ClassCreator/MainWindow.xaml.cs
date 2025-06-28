using ClassCreator.Models;
using ClassCreator.Services;
using ClassCreator.ViewModels;
using System.Diagnostics;
using System.IO;
using System.Windows;


namespace ClassCreator
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private MainViewModel ViewModel => DataContext as MainViewModel;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void OnCreateFolderClick(object sender, RoutedEventArgs e)
        {
            if (ViewModel.SelectedModule == null)
                return;

            var selectedItem = ProjectTreeView.SelectedItem as string;
            if (selectedItem == null)
                return;

            string fullPath = Path.Combine(ViewModel.SelectedModule.Path, selectedItem.TrimStart('\\'), "NovaPasta");

            Directory.CreateDirectory(fullPath);
            ViewModel.SelectedModule = ViewModel.SelectedModule; // Refresh TreeView
        }

        private void OnCreateClassClick(object sender, RoutedEventArgs e)
        {
            if (ViewModel.SelectedModule == null || string.IsNullOrWhiteSpace(ViewModel.ClassName))
            {
                MessageBox.Show("Selecione um módulo e preencha o nome da classe.");
                return;
            }

            string targetPath = Path.Combine(ViewModel.SelectedModule.Path, ViewModel.ClassName);

            if (Directory.Exists(targetPath) || File.Exists(targetPath + ".hpp"))
            {
                MessageBox.Show("Já existe uma classe/pasta com esse nome.");
                return;
            }

            Directory.CreateDirectory(targetPath);

            string hppContent = $"#pragma once\n\nclass {ViewModel.ClassPrefix}{ViewModel.ClassName} : public {ViewModel.BaseClass}\n{{\npublic:\n    {ViewModel.ClassPrefix}{ViewModel.ClassName}();\n}};";
            string cppContent = $"#include \"{ViewModel.ClassName}.hpp\"\n\n{ViewModel.ClassPrefix}{ViewModel.ClassName}::{ViewModel.ClassPrefix}{ViewModel.ClassName}() {{}}";
            string generatedContent = $"// Arquivo gerado automaticamente\n// {ViewModel.ClassName}-generated.hpp";

            File.WriteAllText(Path.Combine(targetPath, $"{ViewModel.ClassName}.hpp"), hppContent);
            File.WriteAllText(Path.Combine(targetPath, $"{ViewModel.ClassName}.cpp"), cppContent);
            File.WriteAllText(Path.Combine(targetPath, $"{ViewModel.ClassName}-generated.hpp"), generatedContent);

            ViewModel.SelectedModule = ViewModel.SelectedModule; // Refresh TreeView

            PreviewBox.Text = $"{hppContent}\n\n{cppContent}\n\n{generatedContent}";

            MessageBox.Show("Classe criada com sucesso!");
        }

        private void OnGenerateClassClick(object sender, RoutedEventArgs e)
        {
            if (DataContext is MainViewModel vm && vm.SelectedModule != null)
            {
                // Raiz do módulo
                string selectedModulePath = vm.SelectedModule.Path;

                // Subpasta selecionada - pode ser null ou vazia
                string selectedSubfolder = vm.SelectedFolderPath;

                string targetFolder = Path.Combine(vm.SelectedModule.Path, vm.SelectedFolderPath ?? "");

               
                ClassGenerator.GenerateClass(
                    folderPath: selectedSubfolder,      // pasta onde .hpp e .cpp vão
                    moduleRootPath: selectedModulePath, // raiz do módulo, onde fica "Generated"
                    className: vm.ClassName,
                    prefix: vm.ClassPrefix,
                    baseClass: vm.BaseClass,
                    namespaceName: vm.NamespaceName,
                    exportMacro: vm.ExportMacro,
                    copyBehavior: ParseBehavior(vm.SelectedCopyBehavior),
                    moveBehavior: ParseBehavior(vm.SelectedMoveBehavior),
                    generateEquality: vm.GenerateEquality,
                    baseClassInclude: vm.GetBaseClassInclude(vm.BaseClass),
                    vm.SelectedModule.PCH
                );

                // Agora executa o .bat do módulo
                RunGenerateProjectBat(vm.SelectedModule.GenerateBatPath);

                MessageBox.Show("Classe gerada com sucesso!");
                vm.LoadDirectoryTree(); // Atualiza TreeView
            }
        }

        private void RunGenerateProjectBat(string batPath)
        {
            if (string.IsNullOrWhiteSpace(batPath) || !File.Exists(batPath))
            {
                MessageBox.Show("Arquivo .bat para geração de projeto não encontrado.", "Erro", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            try
            {
                var processInfo = new ProcessStartInfo
                {
                    FileName = batPath,
                    WorkingDirectory = Path.GetDirectoryName(batPath),
                    UseShellExecute = true
                };

                Process.Start(processInfo);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Erro ao executar o .bat: {ex.Message}", "Erro", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private SpecialFunctionBehavior ParseBehavior(string behavior)
        {
            return behavior switch
            {
                "Default" => SpecialFunctionBehavior.Default,
                "Delete" => SpecialFunctionBehavior.Delete,
                _ => SpecialFunctionBehavior.None,
            };
        }

        private void ProjectTreeView_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            if (DataContext is MainViewModel vm && e.NewValue is TreeItem item)
            {
                // Só aceita pastas para criar arquivos dentro
                if (item.Type == TreeItemType.Folder)
                {
                    // Defina a pasta completa absoluta (exemplo)
                    vm.SelectedFolderPath = item.Path;
                }
            }
        }
    }
}