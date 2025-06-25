using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ClassCreator.Services
{
    public enum SpecialFunctionBehavior
    {
        Default,
        Delete,
        None
    }

    public static class ClassGenerator
    {

        public static void GenerateClass(string folderPath, string moduleRootPath, string className, string prefix, string baseClass, string namespaceName,
                                 string exportMacro, SpecialFunctionBehavior copyBehavior, SpecialFunctionBehavior moveBehavior, bool generateEquality, string baseClassInclude, string PCH)
        {
            string fullClassName = $"{prefix}{className}";
            string baseClassDecl = string.IsNullOrEmpty(baseClass) ? "" : $" : public {baseClass}";

            string namespaceOpen = string.IsNullOrEmpty(namespaceName) ? "" : $"namespace {namespaceName}\n{{\n";
            string namespaceClose = string.IsNullOrEmpty(namespaceName) ? "" : "}\n";

            string macro = string.IsNullOrEmpty(exportMacro) ? "" : $"{exportMacro} ";

            // Gera cabeçalho de comentário para os arquivos
            string summaryComment = $@"/*********************************************************************
 *   File: {className}.hpp
 *  Brief: 
 * 
 * Author: Kleyton Lopes
 *   Date: {DateTime.Now:MMMM yyyy}
 * 
 * Copyright (c) {DateTime.Now:yyyy} Kyrnness. All rights reserved. 
 *********************************************************************/";

            string summaryCommentCpp = summaryComment.Replace($"{className}.hpp", $"{className}.cpp");
            string summaryCommentGenerated = summaryComment.Replace($"{className}.hpp", $"{className}-generated.hpp");
            string[] splitPath = folderPath.Split("\\");
            string defineName = $"K_{splitPath.Last().ToUpper()}_{className.ToUpper()}_HPP";
            ;
            // --- HEADER FILE (.hpp) ---
            StringBuilder hppContent = new StringBuilder();
            hppContent.AppendLine(summaryComment);
            hppContent.AppendLine();
            hppContent.AppendLine("#pragma once");
            hppContent.AppendLine();
            hppContent.AppendLine($"#ifndef {defineName}");
            hppContent.AppendLine($"#define {defineName}");
            hppContent.AppendLine();

            if(!string.IsNullOrEmpty(baseClassInclude))
            {
                hppContent.AppendLine(baseClassInclude);
            }

            //hppContent.AppendLine($"#include \"{className}-generated.hpp\"");
            hppContent.AppendLine("#include \"Core/MinimalCore.hpp\"");
            hppContent.AppendLine();
            hppContent.Append(namespaceOpen);
            hppContent.AppendLine($"{macro}class {fullClassName}{baseClassDecl}");
            hppContent.AppendLine("{");
            
            if(!string.IsNullOrWhiteSpace(baseClass))
                hppContent.AppendLine($"    using Super = {baseClass};");

            hppContent.AppendLine("public:");
            hppContent.AppendLine($"    {fullClassName}();");
            hppContent.AppendLine($"    virtual ~{fullClassName}();");
            hppContent.AppendLine();

            if (copyBehavior != SpecialFunctionBehavior.None)
                hppContent.AppendLine($"    {fullClassName}(const {fullClassName}& other) = {copyBehavior.ToString().ToLower()};");

            if (moveBehavior != SpecialFunctionBehavior.None)
                hppContent.AppendLine($"    {fullClassName}({fullClassName}&& other) noexcept = {moveBehavior.ToString().ToLower()};");

            if (copyBehavior != SpecialFunctionBehavior.None)
                hppContent.AppendLine($"    {fullClassName}& operator=(const {fullClassName}& other) = {copyBehavior.ToString().ToLower()};");

            if (moveBehavior != SpecialFunctionBehavior.None)
                hppContent.AppendLine($"    {fullClassName}& operator=({fullClassName}&& other) noexcept = {moveBehavior.ToString().ToLower()};");

            if (generateEquality)
                hppContent.AppendLine($"    bool operator==(const {fullClassName}& other) const;");

            hppContent.AppendLine();
            hppContent.AppendLine("protected:");
            hppContent.AppendLine();
            hppContent.AppendLine("private:");
            hppContent.AppendLine();
            hppContent.AppendLine("};");
            hppContent.AppendLine();
            hppContent.AppendLine($"#endif // {defineName}");
            hppContent.Append(namespaceClose);

            // --- CPP FILE ---
            StringBuilder cppContent = new StringBuilder();
            cppContent.AppendLine(summaryCommentCpp);
            cppContent.AppendLine();
            if(!string.IsNullOrWhiteSpace(PCH))
                cppContent.AppendLine($"#include \"{PCH}\"");
            cppContent.AppendLine($"#include \"{className}.hpp\"");
            cppContent.AppendLine();
            cppContent.Append(namespaceOpen);
            cppContent.AppendLine($"{fullClassName}::{fullClassName}() {{}}");
            cppContent.AppendLine();
            cppContent.AppendLine($"{fullClassName}::~{fullClassName}() {{}}");
            cppContent.AppendLine();

            //if (copyBehavior != SpecialFunctionBehavior.None)
            //{
            //    cppContent.AppendLine($"{fullClassName}::{fullClassName}(const {fullClassName}& other) = {copyBehavior.ToString().ToLower()};");
            //    cppContent.AppendLine();
            //    cppContent.AppendLine($"{fullClassName}& {fullClassName}::operator=(const {fullClassName}& other) = {copyBehavior.ToString().ToLower()};");
            //    cppContent.AppendLine();
            //}
            //
            //if (moveBehavior != SpecialFunctionBehavior.None)
            //{
            //    cppContent.AppendLine($"{fullClassName}::{fullClassName}({fullClassName}&& other) noexcept = {moveBehavior.ToString().ToLower()};");
            //    cppContent.AppendLine();
            //    cppContent.AppendLine($"{fullClassName}& {fullClassName}::operator=({fullClassName}&& other) noexcept = {moveBehavior.ToString().ToLower()};");
            //    cppContent.AppendLine();
            //}

            if (generateEquality)
            {
                cppContent.AppendLine($"bool {fullClassName}::operator==(const {fullClassName}& other) const");
                cppContent.AppendLine("{");
                cppContent.AppendLine("    // TODO: Compare members");
                cppContent.AppendLine("    return true;");
                cppContent.AppendLine("}");
            }

            cppContent.AppendLine(namespaceClose);

            // --- GENERATED HEADER ---

            //StringBuilder generatedContent = new StringBuilder();
            //generatedContent.AppendLine(summaryCommentGenerated);
            //generatedContent.AppendLine();
            //generatedContent.AppendLine($"// Macros ou definições geradas para {fullClassName}");
            //generatedContent.AppendLine();
            //generatedContent.AppendLine("#undef CURRENT_CLASS_IDENTITY_FILE");
            //generatedContent.AppendLine($"#define CURRENT_CLASS_IDENTITY_FILE hpp_{splitPath.Last().ToUpper()}_{className.ToUpper()}");

            // --- SALVAR ARQUIVOS ---

            string hppPath = Path.Combine(folderPath, $"{className}.hpp");
            string cppPath = Path.Combine(folderPath, $"{className}.cpp");

            //string generatedFolder = Path.Combine(moduleRootPath, "Generated");
            //if (!Directory.Exists(generatedFolder))
            //    Directory.CreateDirectory(generatedFolder);

            //string generatedPath = Path.Combine(generatedFolder, $"{className}-generated.hpp");

            File.WriteAllText(hppPath, hppContent.ToString());
            File.WriteAllText(cppPath, cppContent.ToString());
            ///File.WriteAllText(generatedPath, generatedContent.ToString());
        }
    }
}
