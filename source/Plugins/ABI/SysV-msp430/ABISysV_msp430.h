//===-- ABISysV_msp430.h ----------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef liblldb_ABISysV_msp430_h_
#define liblldb_ABISysV_msp430_h_

// C Includes
// C++ Includes
// Other libraries and framework includes
// Project includes
#include "lldb/lldb-private.h"
#include "lldb/Target/ABI.h"

class ABISysV_msp430 :
    public lldb_private::ABI
{
public:
    ~ABISysV_msp430() override = default;

    size_t
    GetRedZoneSize() const override;

    bool
    PrepareTrivialCall(lldb_private::Thread &thread,
                       lldb::addr_t sp,
                       lldb::addr_t functionAddress,
                       lldb::addr_t returnAddress,
                       llvm::ArrayRef<lldb::addr_t> args) const override;

    bool
    GetArgumentValues(lldb_private::Thread &thread,
                       lldb_private::ValueList &values) const override;
    
    lldb_private::Error
    SetReturnValueObject(lldb::StackFrameSP &frame_sp,
                         lldb::ValueObjectSP &new_value) override;

    lldb::ValueObjectSP
    GetReturnValueObjectImpl(lldb_private::Thread &thread,
                             lldb_private::CompilerType &type) const override;

    bool
    CreateFunctionEntryUnwindPlan(lldb_private::UnwindPlan &unwind_plan) override;
    
    bool
    CreateDefaultUnwindPlan(lldb_private::UnwindPlan &unwind_plan) override;
        
    bool
    RegisterIsVolatile(const lldb_private::RegisterInfo *reg_info) override;

    bool
    CallFrameAddressIsValid(lldb::addr_t cfa) override
    {
        // Make sure the stack call frame addresses are 2 byte aligned
        if (cfa & 0x01)
            return false;   // Not 2 byte aligned
        if (cfa == 0)
            return false;   // Zero is not a valid stack address
        return true;
    }
    
    bool
    CodeAddressIsValid(lldb::addr_t pc) override
    {
        // We have a 64 bit address space, so anything is valid as opcodes
        // aren't fixed width...
        return true;
    }

    const lldb_private::RegisterInfo *
    GetRegisterInfoArray(uint32_t &count) override;

    //------------------------------------------------------------------
    // Static Functions
    //------------------------------------------------------------------

    static void
    Initialize();

    static void
    Terminate();

    static lldb::ABISP
    CreateInstance ( const lldb_private::ArchSpec &arch );

    static lldb_private::ConstString
    GetPluginNameStatic();
    
    //------------------------------------------------------------------
    // PluginInterface protocol
    //------------------------------------------------------------------

    lldb_private::ConstString
    GetPluginName() override;

    uint32_t
    GetPluginVersion() override;

protected:
    void
    CreateRegisterMapIfNeeded();

    lldb::ValueObjectSP
    GetReturnValueObjectSimple(lldb_private::Thread &thread,
                               lldb_private::CompilerType &ast_type) const;

    bool
    RegisterIsCalleeSaved (const lldb_private::RegisterInfo *reg_info);

private:
    ABISysV_msp430() :
        lldb_private::ABI() 
    {
         // Call CreateInstance instead.
    }
};

#endif // liblldb_ABISysV_msp430_h_
