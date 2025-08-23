# ========================================
# Git Helper Script for STM32 Project
# ========================================

Write-Host "STM32 Quadruped Robot Git Helper" -ForegroundColor Green
Write-Host "=================================" -ForegroundColor Green

# 显示当前选项
Write-Host "`nAvailable options:" -ForegroundColor Yellow
Write-Host "1. Show status (ignore build files)" -ForegroundColor Cyan
Write-Host "2. Add only source code changes" -ForegroundColor Cyan
Write-Host "3. Commit source code changes" -ForegroundColor Cyan
Write-Host "4. Skip tracking build files (advanced)" -ForegroundColor Cyan
Write-Host "5. Show what files are tracked" -ForegroundColor Cyan

$choice = Read-Host "`nEnter your choice (1-5)"

switch ($choice) {
    "1" {
        Write-Host "`nShowing Git status (filtering out build files)..." -ForegroundColor Green
        git status --porcelain | Where-Object { 
            $_ -notmatch '\.(o|crf|d|axf|hex|htm|lnp|map|dep)' 
        } | ForEach-Object {
            Write-Host $_ -ForegroundColor White
        }
    }
    
    "2" {
        Write-Host "`nAdding only source code files..." -ForegroundColor Green
        
        # 添加源代码文件
        git add *.c *.h *.cpp *.hpp 2>$null
        git add Src/ Inc/ App/ Task/ 2>$null
        git add *.ioc *.uvprojx Makefile CMakeLists.txt 2>$null
        git add README.md .gitignore 2>$null
        
        Write-Host "Source code files added successfully!" -ForegroundColor Green
        git status --short
    }
    
    "3" {
        Write-Host "`nCommitting source code changes..." -ForegroundColor Green
        $message = Read-Host "Enter commit message"
        if ($message) {
            git commit -m $message
            Write-Host "Changes committed successfully!" -ForegroundColor Green
        } else {
            Write-Host "Commit cancelled - no message provided." -ForegroundColor Red
        }
    }
    
    "4" {
        Write-Host "`nWarning: This will tell Git to ignore changes to build files." -ForegroundColor Yellow
        Write-Host "This is advanced and may cause issues when switching branches." -ForegroundColor Yellow
        $confirm = Read-Host "Continue? (y/N)"
        
        if ($confirm -eq "y" -or $confirm -eq "Y") {
            Write-Host "Setting skip-worktree for build files..." -ForegroundColor Green
            
            git ls-files | Where-Object { 
                $_ -match '\.(o|crf|d|axf|hex|htm|lnp|map|dep)$' 
            } | ForEach-Object {
                git update-index --skip-worktree $_
                Write-Host "Skipping: $_" -ForegroundColor Gray
            }
            
            Write-Host "Build files are now ignored for changes." -ForegroundColor Green
            Write-Host "Use 'git update-index --no-skip-worktree <file>' to undo." -ForegroundColor Yellow
        }
    }
    
    "5" {
        Write-Host "`nFiles currently tracked by Git:" -ForegroundColor Green
        Write-Host "Source Files:" -ForegroundColor Cyan
        git ls-files | Where-Object { 
            $_ -match '\.(c|h|cpp|hpp|ioc|uvprojx)$' 
        } | Sort-Object | ForEach-Object {
            Write-Host "  $_" -ForegroundColor White
        }
        
        Write-Host "`nBuild Files:" -ForegroundColor Yellow
        git ls-files | Where-Object { 
            $_ -match '\.(o|crf|d|axf|hex|htm|lnp|map|dep)$' 
        } | Sort-Object | ForEach-Object {
            Write-Host "  $_" -ForegroundColor Gray
        }
    }
    
    default {
        Write-Host "Invalid choice. Please run the script again." -ForegroundColor Red
    }
}

Write-Host "`nDone!" -ForegroundColor Green
